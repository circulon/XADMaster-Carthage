/*
 * XADArchiveParser.h
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
#import <Foundation/Foundation.h>
#import "XADException.h"
#import "XADString.h"
#import "XADPath.h"
#import "XADRegex.h"
#import "CSHandle.h"
#import "XADSkipHandle.h"
#import "XADResourceFork.h"
#import "Checksums.h"

extern NSString *const XADFileNameKey;
extern NSString *const XADCommentKey;
extern NSString *const XADFileSizeKey;
extern NSString *const XADCompressedSizeKey;
extern NSString *const XADCompressionNameKey;

extern NSString *const XADLastModificationDateKey;
extern NSString *const XADLastAccessDateKey;
extern NSString *const XADLastAttributeChangeDateKey;
extern NSString *const XADLastBackupDateKey;
extern NSString *const XADCreationDateKey;

extern NSString *const XADIsDirectoryKey;
extern NSString *const XADIsResourceForkKey;
extern NSString *const XADIsArchiveKey;
extern NSString *const XADIsHiddenKey;
extern NSString *const XADIsLinkKey;
extern NSString *const XADIsHardLinkKey;
extern NSString *const XADLinkDestinationKey;
extern NSString *const XADIsCharacterDeviceKey;
extern NSString *const XADIsBlockDeviceKey;
extern NSString *const XADDeviceMajorKey;
extern NSString *const XADDeviceMinorKey;
extern NSString *const XADIsFIFOKey;
extern NSString *const XADIsEncryptedKey;
extern NSString *const XADIsCorruptedKey;

extern NSString *const XADExtendedAttributesKey;
extern NSString *const XADFileTypeKey;
extern NSString *const XADFileCreatorKey;
extern NSString *const XADFinderFlagsKey;
extern NSString *const XADFinderInfoKey;
extern NSString *const XADPosixPermissionsKey;
extern NSString *const XADPosixUserKey;
extern NSString *const XADPosixGroupKey;
extern NSString *const XADPosixUserNameKey;
extern NSString *const XADPosixGroupNameKey;
extern NSString *const XADDOSFileAttributesKey;
extern NSString *const XADWindowsFileAttributesKey;
extern NSString *const XADAmigaProtectionBitsKey;

extern NSString *const XADIndexKey;
extern NSString *const XADDataOffsetKey;
extern NSString *const XADDataLengthKey;
extern NSString *const XADSkipOffsetKey;
extern NSString *const XADSkipLengthKey;

extern NSString *const XADIsSolidKey;
extern NSString *const XADFirstSolidIndexKey;
extern NSString *const XADFirstSolidEntryKey;
extern NSString *const XADNextSolidIndexKey;
extern NSString *const XADNextSolidEntryKey;
extern NSString *const XADSolidObjectKey;
extern NSString *const XADSolidOffsetKey;
extern NSString *const XADSolidLengthKey;

// Archive properties only
extern NSString *const XADArchiveNameKey;
extern NSString *const XADVolumesKey;
extern NSString *const XADVolumeScanningFailedKey;
extern NSString *const XADDiskLabelKey;

extern NSString *const XADSignatureOffset;
extern NSString *const XADParserClass;

@protocol XADArchiveParserDelegate;

@interface XADArchiveParser:NSObject
{
	CSHandle *sourcehandle;
	XADSkipHandle *skiphandle;
	XADResourceFork *resourcefork;

	id<XADArchiveParserDelegate> delegate;
	NSString *password;
	NSString *passwordencodingname;
	BOOL caresaboutpasswordencoding;

	NSMutableDictionary *properties;
	XADStringSource *stringsource;

	int currindex;

	id parsersolidobj;
	NSMutableDictionary *firstsoliddict,*prevsoliddict;
	id currsolidobj;
	CSHandle *currsolidhandle;
	BOOL forcesolid;

	BOOL shouldstop;
}

+(void)initialize;
+(Class)archiveParserClassForHandle:(CSHandle *)handle firstBytes:(NSData *)header
resourceFork:(XADResourceFork *)fork name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;
+ (Class)archiveParserFromParsersWithFloatingSignature:(NSArray *)parsers forHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;
+ (BOOL)isValidParserClass:(Class)parserClass forHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;

+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle name:(NSString *)name;
+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle name:(NSString *)name error:(XADError *)errorptr;
+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle resourceFork:(XADResourceFork *)fork name:(NSString *)name;
+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle resourceFork:(XADResourceFork *)fork name:(NSString *)name error:(XADError *)errorptr;
+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name;
+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name error:(XADError *)errorptr;
+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header resourceFork:(XADResourceFork *)fork name:(NSString *)name;
+(XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header resourceFork:(XADResourceFork *)fork name:(NSString *)name error:(XADError *)errorptr;
+(XADArchiveParser *)archiveParserForPath:(NSString *)filename;
+(XADArchiveParser *)archiveParserForPath:(NSString *)filename error:(XADError *)errorptr;
+(XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum;
+(XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum error:(XADError *)errorptr;
+(XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry resourceForkDictionary:(NSDictionary *)forkentry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum;
+(XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry resourceForkDictionary:(NSDictionary *)forkentry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum error:(XADError *)errorptr;
 
-(id)init;
-(void)dealloc;

@property (nonatomic, retain) CSHandle *handle;
@property (retain) XADResourceFork *resourceFork;
-(NSString *)name;
-(void)setName:(NSString *)newname;
-(NSString *)filename;
-(void)setFilename:(NSString *)filename;
-(NSArray *)allFilenames;
-(void)setAllFilenames:(NSArray *)newnames;

@property (assign) id<XADArchiveParserDelegate> delegate;

@property (readonly, copy) NSDictionary *properties;
-(NSString *)currentFilename;

@property (readonly, nonatomic, getter=isEncrypted) BOOL encrypted;
@property (nonatomic, copy) NSString *password;
@property (nonatomic, readonly) BOOL hasPassword;

-(NSString *)encodingName;
@property (nonatomic, readonly) float encodingConfidence;
-(void)setEncodingName:(NSString *)encodingname;
@property (readonly) BOOL caresAboutPasswordEncoding;
@property (nonatomic, retain) NSString *passwordEncodingName;
@property (readonly, retain) XADStringSource *stringSource;

-(XADString *)linkDestinationForDictionary:(NSDictionary *)dict;
-(XADString *)linkDestinationForDictionary:(NSDictionary *)dict error:(XADError *)errorptr;
-(NSDictionary *)extendedAttributesForDictionary:(NSDictionary *)dict;
-(NSData *)finderInfoForDictionary:(NSDictionary *)dict;

-(BOOL)wasStopped;

-(BOOL)hasChecksum;
-(BOOL)testChecksum;
-(XADError)testChecksumWithoutExceptions;



// Internal functions

+(NSArray *)scanForVolumesWithFilename:(NSString *)filename regex:(XADRegex *)regex;
+(NSArray *)scanForVolumesWithFilename:(NSString *)filename
regex:(XADRegex *)regex firstFileExtension:(NSString *)firstext;

-(BOOL)shouldKeepParsing;

-(CSHandle *)handleAtDataOffsetForDictionary:(NSDictionary *)dict;
-(XADSkipHandle *)skipHandle;
-(CSHandle *)zeroLengthHandleWithChecksum:(BOOL)checksum;
-(CSHandle *)subHandleFromSolidStreamForEntryWithDictionary:(NSDictionary *)dict;

-(BOOL)hasVolumes;
-(NSArray *)volumeSizes;
-(CSHandle *)currentHandle;

-(void)setObject:(id)object forPropertyKey:(NSString *)key;
-(void)addPropertiesFromDictionary:(NSDictionary *)dict;
-(void)setIsMacArchive:(BOOL)ismac;

-(void)addEntryWithDictionary:(NSMutableDictionary *)dict;
-(void)addEntryWithDictionary:(NSMutableDictionary *)dict retainPosition:(BOOL)retainpos;

-(XADString *)XADStringWithString:(NSString *)string;
-(XADString *)XADStringWithData:(NSData *)data;
-(XADString *)XADStringWithData:(NSData *)data encodingName:(NSString *)encoding;
-(XADString *)XADStringWithBytes:(const void *)bytes length:(int)length;
-(XADString *)XADStringWithBytes:(const void *)bytes length:(int)length encodingName:(NSString *)encoding;
-(XADString *)XADStringWithCString:(const char *)cstring;
-(XADString *)XADStringWithCString:(const char *)cstring encodingName:(NSString *)encoding;

-(XADPath *)XADPath;
-(XADPath *)XADPathWithString:(NSString *)string;
-(XADPath *)XADPathWithUnseparatedString:(NSString *)string;
-(XADPath *)XADPathWithData:(NSData *)data separators:(const char *)separators;
-(XADPath *)XADPathWithData:(NSData *)data encodingName:(NSString *)encoding separators:(const char *)separators;
-(XADPath *)XADPathWithBytes:(const void *)bytes length:(int)length separators:(const char *)separators;
-(XADPath *)XADPathWithBytes:(const void *)bytes length:(int)length encodingName:(NSString *)encoding separators:(const char *)separators;
-(XADPath *)XADPathWithCString:(const char *)cstring separators:(const char *)separators;
-(XADPath *)XADPathWithCString:(const char *)cstring encodingName:(NSString *)encoding separators:(const char *)separators;

-(NSData *)encodedPassword;
-(const char *)encodedCStringPassword;

-(void)reportInterestingFileWithReason:(NSString *)reason,...;



// Subclasses implement these:

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;
+(NSArray *)volumesForHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name;

-(void)parse;
-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

-(CSHandle *)handleForSolidStreamWithObject:(id)obj wantChecksum:(BOOL)checksum;

// Exception-free wrappers for subclass methods:
// parseWithoutExceptions will in addition return XADBreakError if the delegate
// requested parsing to stop.

-(XADError)parseWithoutExceptions;
-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum error:(XADError *)errorptr;

@end

@protocol XADArchiveParserDelegate <NSObject>
@optional

-(void)archiveParser:(XADArchiveParser *)parser foundEntryWithDictionary:(NSDictionary *)dict;
-(BOOL)archiveParsingShouldStop:(XADArchiveParser *)parser;
-(void)archiveParserNeedsPassword:(XADArchiveParser *)parser;
-(void)archiveParser:(XADArchiveParser *)parser findsFileInterestingForReason:(NSString *)reason;

@end
