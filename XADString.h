/*
 * XADString.h
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

@class XADStringSource,UniversalDetector;


extern NSString *const XADUTF8StringEncodingName;
extern NSString *const XADASCIIStringEncodingName;

extern NSString *const XADISOLatin1StringEncodingName;
extern NSString *const XADISOLatin2StringEncodingName;
extern NSString *const XADISOLatin3StringEncodingName;
extern NSString *const XADISOLatin4StringEncodingName;
extern NSString *const XADISOLatin5StringEncodingName;
extern NSString *const XADISOLatin6StringEncodingName;
extern NSString *const XADISOLatin7StringEncodingName;
extern NSString *const XADISOLatin8StringEncodingName;
extern NSString *const XADISOLatin9StringEncodingName;
extern NSString *const XADISOLatin10StringEncodingName;
extern NSString *const XADISOLatin11StringEncodingName;
extern NSString *const XADISOLatin12StringEncodingName;
extern NSString *const XADISOLatin13StringEncodingName;
extern NSString *const XADISOLatin14StringEncodingName;
extern NSString *const XADISOLatin15StringEncodingName;
extern NSString *const XADISOLatin16StringEncodingName;

extern NSString *const XADShiftJISStringEncodingName;

extern NSString *const XADWindowsCP1250StringEncodingName;
extern NSString *const XADWindowsCP1251StringEncodingName;
extern NSString *const XADWindowsCP1252StringEncodingName;
extern NSString *const XADWindowsCP1253StringEncodingName;
extern NSString *const XADWindowsCP1254StringEncodingName;

extern NSString *const XADMacOSRomanStringEncodingName;
extern NSString *const XADMacOSJapaneseStringEncodingName;
extern NSString *const XADMacOSTraditionalChineseStringEncodingName;
extern NSString *const XADMacOSKoreanStringEncodingName;
extern NSString *const XADMacOSArabicStringEncodingName;
extern NSString *const XADMacOSHebrewStringEncodingName;
extern NSString *const XADMacOSGreekStringEncodingName;
extern NSString *const XADMacOSCyrillicStringEncodingName;
extern NSString *const XADMacOSSimplifiedChineseStringEncodingName;
extern NSString *const XADMacOSRomanianStringEncodingName;
extern NSString *const XADMacOSUkranianStringEncodingName;
extern NSString *const XADMacOSThaiStringEncodingName;
extern NSString *const XADMacOSCentralEuropeanRomanStringEncodingName;
extern NSString *const XADMacOSIcelandicStringEncodingName;
extern NSString *const XADMacOSTurkishStringEncodingName;
extern NSString *const XADMacOSCroatianStringEncodingName;


@protocol XADString <NSObject>

-(BOOL)canDecodeWithEncodingName:(NSString *)encoding;
-(NSString *)string;
-(NSString *)stringWithEncodingName:(NSString *)encoding;
-(NSData *)data;

-(BOOL)encodingIsKnown;
-(NSString *)encodingName;
-(float)confidence;

-(XADStringSource *)source;

#ifdef __APPLE__
-(BOOL)canDecodeWithEncoding:(NSStringEncoding)encoding;
-(NSString *)stringWithEncoding:(NSStringEncoding)encoding;
-(NSStringEncoding)encoding;
#endif

@end



@interface XADString:NSObject <XADString,NSCopying>
{
	NSData *data;
	NSString *string;
	XADStringSource *source;
}

+(XADString *)XADStringWithString:(NSString *)string;
+(XADString *)analyzedXADStringWithData:(NSData *)bytedata source:(XADStringSource *)stringsource;
+(XADString *)decodedXADStringWithData:(NSData *)bytedata encodingName:(NSString *)encoding;

+(NSString *)escapedStringForData:(NSData *)data encodingName:(NSString *)encoding;
+(NSString *)escapedStringForBytes:(const void *)bytes length:(size_t)length encodingName:(NSString *)encoding;
+(NSString *)escapedASCIIStringForBytes:(const void *)bytes length:(size_t)length;
+(NSData *)escapedASCIIDataForString:(NSString *)string;

-(id)initWithData:(NSData *)bytedata source:(XADStringSource *)stringsource;
-(id)initWithString:(NSString *)knownstring;
-(void)dealloc;

-(BOOL)canDecodeWithEncodingName:(NSString *)encoding;
-(NSString *)string;
-(NSString *)stringWithEncodingName:(NSString *)encoding;
-(NSData *)data;

-(BOOL)encodingIsKnown;
-(NSString *)encodingName;
-(float)confidence;

-(XADStringSource *)source;

-(BOOL)hasASCIIPrefix:(NSString *)asciiprefix;
-(XADString *)XADStringByStrippingASCIIPrefixOfLength:(int)length;

-(BOOL)isEqual:(id)other;
-(NSUInteger)hash;

-(NSString *)description;
-(id)copyWithZone:(NSZone *)zone;

#ifdef __APPLE__
-(BOOL)canDecodeWithEncoding:(NSStringEncoding)encoding;
-(NSString *)stringWithEncoding:(NSStringEncoding)encoding;
-(NSStringEncoding)encoding;
#endif

@end

@interface XADString (PlatformSpecific)

+(BOOL)canDecodeData:(NSData *)data encodingName:(NSString *)encoding;
+(BOOL)canDecodeBytes:(const void *)bytes length:(size_t)length encodingName:(NSString *)encoding;
+(NSString *)stringForData:(NSData *)data encodingName:(NSString *)encoding;
+(NSString *)stringForBytes:(const void *)bytes length:(size_t)length encodingName:(NSString *)encoding;
+(NSData *)dataForString:(NSString *)string encodingName:(NSString *)encoding;
+(NSArray *)availableEncodingNames;

#ifdef __APPLE__
+(NSString *)encodingNameForEncoding:(NSStringEncoding)encoding;
+(NSStringEncoding)encodingForEncodingName:(NSString *)encoding;
#endif

@end




@interface XADStringSource:NSObject
{
	UniversalDetector *detector;
	NSString *fixedencodingname;
	BOOL mac,hasanalyzeddata;

	#ifdef __APPLE__
	NSStringEncoding fixedencoding;
	#endif
}

-(id)init;
-(void)dealloc;

-(void)analyzeData:(NSData *)data;

-(BOOL)hasAnalyzedData;
-(NSString *)encodingName;
-(float)confidence;
-(UniversalDetector *)detector;

-(void)setFixedEncodingName:(NSString *)encodingname;
-(BOOL)hasFixedEncoding;
-(void)setPrefersMacEncodings:(BOOL)prefermac;

#ifdef __APPLE__
-(NSStringEncoding)encoding;
-(void)setFixedEncoding:(NSStringEncoding)encoding;
#endif

@end
