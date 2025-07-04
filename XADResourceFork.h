/*
 * XADResourceFork.h
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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wquoted-include-in-framework-header"
#import "CSHandle.h"
#import "XADException.h"
#pragma clang diagnostic pop

XADEXPORT
@interface XADResourceFork:NSObject
{
	NSDictionary *resources;
}

+(XADResourceFork *)resourceForkWithHandle:(CSHandle *)handle;
+(XADResourceFork *)resourceForkWithHandle:(CSHandle *)handle error:(XADError *)errorptr;

-(id)init;
-(void)dealloc;

-(void)parseFromHandle:(CSHandle *)handle;
-(NSData *)resourceDataForType:(uint32_t)type identifier:(int)identifier;

-(NSMutableDictionary *)_parseResourceDataFromHandle:(CSHandle *)handle;
-(NSDictionary *)_parseMapFromHandle:(CSHandle *)handle withDataObjects:(NSMutableDictionary *)dataobjects;
-(NSDictionary *)_parseReferencesFromHandle:(CSHandle *)handle count:(int)count;

@end
