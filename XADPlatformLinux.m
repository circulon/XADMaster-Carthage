/*
 * XADPlatformLinux.m
 *
 * Copyright (c) 2017-present, MacPaw Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */
#import "XADPlatform.h"
#import "NSDateXAD.h"

#import <fcntl.h>
#import <unistd.h>
#import <sys/stat.h>
#import <sys/time.h>




@implementation XADPlatform

//
// Archive entry extraction.
//

+(XADError)extractResourceForkEntryWithDictionary:(NSDictionary *)dict
unarchiver:(XADUnarchiver *)unarchiver toPath:(NSString *)destpath
{
	return XADErrorNotSupported;
}

+(XADError)updateFileAttributesAtPath:(NSString *)path
forEntryWithDictionary:(NSDictionary *)dict parser:(XADArchiveParser *)parser
preservePermissions:(BOOL)preservepermissions
{
	const char *cpath=[path fileSystemRepresentation];

	NSNumber *linknum=[dict objectForKey:XADIsLinkKey];
	BOOL islink=linknum&&[linknum boolValue];

	struct stat st;
	if(lstat(cpath,&st)!=0) return XADErrorOpenFile; // TODO: better error

	// If the file does not have write permissions, change this temporarily
	// and remember to change back.
	BOOL changedpermissions=NO;
	if(!(st.st_mode&S_IWUSR))
	{
		if(!islink) chmod(cpath,0700);
		changedpermissions=YES;
	}

	// Handle timestamps.
	NSDate *modification=[dict objectForKey:XADLastModificationDateKey];
	NSDate *access=[dict objectForKey:XADLastAccessDateKey];

	if(modification||access)
	{
		struct timeval times[2]={
			{st.st_atim.tv_sec,st.st_atim.tv_nsec/1000},
			{st.st_mtim.tv_sec,st.st_mtim.tv_nsec/1000},
		};

		if(access) times[0]=[access timevalStruct];
		if(modification) times[1]=[modification timevalStruct];

		int res=lutimes(cpath,times);
		if(res!=0&&res!=ENOSYS) return XADErrorUnknown; // TODO: better error
	}

	// Handle permissions (or change back to original permissions if they were changed).
	NSNumber *permissions=[dict objectForKey:XADPosixPermissionsKey];
	if(permissions||changedpermissions)
	{
		mode_t mode=st.st_mode;

		if(permissions)
		{
			mode=[permissions unsignedShortValue];
			if(!preservepermissions)
			{
				mode_t mask=umask(022);
				umask(mask); // This is stupid. Is there no sane way to just READ the umask?
				mode&=~(mask|S_ISUID|S_ISGID);

				// Just force read and write flags for all files, no matter what
				// insane archives think.
				mode|=S_IRUSR|S_IWUSR;
			}
		}

		if(!islink)
		if(chmod(cpath,mode&~S_IFMT)!=0) return XADErrorUnknown; // TODO: bette error
	}

	return XADErrorNone;
}

+(XADError)createLinkAtPath:(NSString *)path withDestinationPath:(NSString *)link
{
	struct stat st;
	const char *destcstr=[path fileSystemRepresentation];
	if(lstat(destcstr,&st)==0) unlink(destcstr);
	if(symlink([link fileSystemRepresentation],destcstr)!=0) return XADErrorLink;

	return XADErrorNone;
}




//
// Archive post-processing.
//

+(id)readCloneableMetadataFromPath:(NSString *)path { return nil; }
+(void)writeCloneableMetadata:(id)metadata toPath:(NSString *)path {}

+(BOOL)copyDateFromPath:(NSString *)src toPath:(NSString *)dest
{
	struct stat st;
	const char *csrc=[src fileSystemRepresentation];
	if(stat(csrc,&st)!=0) return NO;

	struct timeval times[2]={
		{st.st_atim.tv_sec,st.st_atim.tv_nsec/1000},
		{st.st_mtim.tv_sec,st.st_mtim.tv_nsec/1000},
	};

	const char *cdest=[dest fileSystemRepresentation];
	if(utimes(cdest,times)!=0) return NO;

	return YES;
}

+(BOOL)resetDateAtPath:(NSString *)path
{
	const char *cpath=[path fileSystemRepresentation];
	if(utimes(cpath,NULL)!=0) return NO;

	return YES;
}




//
// Path functions.
//

+(BOOL)fileExistsAtPath:(NSString *)path { return [self fileExistsAtPath:path isDirectory:NULL]; }

+(BOOL)fileExistsAtPath:(NSString *)path isDirectory:(BOOL *)isdirptr
{
	// [NSFileManager fileExistsAtPath:] is broken. It will happily return NO
	// for some symbolic links. We need to implement our own.

	struct stat st;
	if(lstat([path fileSystemRepresentation],&st)!=0) return NO;

	if(isdirptr)
	{
		if((st.st_mode&S_IFMT)==S_IFDIR) *isdirptr=YES;
		else *isdirptr=NO;
	}

	return YES;
}

+(NSString *)uniqueDirectoryPathWithParentDirectory:(NSString *)parent
{
	// TODO: ensure this path is actually unique.
	NSDate *now=[NSDate date];
	int64_t t=[now timeIntervalSinceReferenceDate]*1000000000;
	pid_t pid=getpid();

	NSString *dirname=[NSString stringWithFormat:@"XADTemp%qd%d",t,pid];

	if(parent) return [parent stringByAppendingPathComponent:dirname];
	else return dirname;
}

+(NSString *)sanitizedPathComponent:(NSString *)component
{
	if([component rangeOfString:@"/"].location==NSNotFound&&
	[component rangeOfString:@"\000"].location==NSNotFound) return component;

	NSMutableString *newstring=[NSMutableString stringWithString:component];
	[newstring replaceOccurrencesOfString:@"/" withString:@"_" options:0 range:NSMakeRange(0,[newstring length])];
	[newstring replaceOccurrencesOfString:@"\000" withString:@"_" options:0 range:NSMakeRange(0,[newstring length])];
	return newstring;
}

+(NSArray *)contentsOfDirectoryAtPath:(NSString *)path
{
	return [[NSFileManager defaultManager] directoryContentsAtPath:path];
}

+(BOOL)moveItemAtPath:(NSString *)src toPath:(NSString *)dest
{
	return [[NSFileManager defaultManager] movePath:src toPath:dest handler:nil];
}

+(BOOL)removeItemAtPath:(NSString *)path
{
	return [[NSFileManager defaultManager] removeFileAtPath:path handler:nil];
}



//
// Resource forks
//

+(CSHandle *)handleForReadingResourceForkAtPath:(NSString *)path { return nil; }

+(CSHandle *)handleForReadingResourceForkAtFileURL:(NSURL *)path { return nil; }

//
// Time functions.
//

+(double)currentTimeInSeconds
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (double)tv.tv_sec+(double)tv.tv_usec/1000000.0;
}

@end
