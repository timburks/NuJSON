/*
Copyright (c) 2007, Stig Brautaset. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

Neither the name of the author nor the names of its contributors may be used
to endorse or promote products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
@mainpage A strict JSON parser and generator for Objective-C

JSON (JavaScript Object Notation) is a lightweight data-interchange
format. This framework provides two apis for parsing and generating
JSON. One standard object-based and a higher level api consisting of
categories added to existing Objective-C classes.

Learn more on the http://code.google.com/p/json-framework project site.
*/

#import <Foundation/Foundation.h>

extern NSString * SBJSONErrorDomain;

enum
{
    EUNSUPPORTED = 1,
    EPARSENUM,
    EPARSE,
    EFRAGMENT,
    ECTRL,
    EUNICODE,
    EDEPTH,
    EESCAPE,
    ETRAILCOMMA,
    ETRAILGARBAGE,
    EEOF,
    EINPUT
};

/**
@brief A strict JSON parser and generator

This is the parser and generator underlying the categories added to
NSString and various other objects.

Objective-C types are mapped to JSON types and back in the following way:

@li NSNull -> Null -> NSNull
@li NSString -> String -> NSMutableString
@li NSArray -> Array -> NSMutableArray
@li NSDictionary -> Object -> NSMutableDictionary
@li NSNumber (-initWithBool:) -> Boolean -> NSNumber -initWithBool:
@li NSNumber -> Number -> NSDecimalNumber

In JSON the keys of an object must be strings. NSDictionary keys need
not be, but attempting to convert an NSDictionary with non-string keys
into JSON will throw an exception.

NSNumber instances created with the +numberWithBool: method are
converted into the JSON boolean "true" and "false" values, and vice
versa. Any other NSNumber instances are converted to a JSON number the
way you would expect. JSON numbers turn into NSDecimalNumber instances,
as we can thus avoid any loss of precision.

Strictly speaking correctly formed JSON text must have <strong>exactly
one top-level container</strong>. (Either an Array or an Object.) Scalars,
i.e. nulls, numbers, booleans and strings, are not valid JSON on their own.
It can be quite convenient to pretend that such fragments are valid
JSON however, and this class lets you do so.

This class does its best to be as strict as possible, both in what it
accepts and what it generates. (Other than the above mentioned support
for JSON fragments.) For example, it does not support trailing commas
in arrays or objects. Nor does it support embedded comments, or
anything else not in the JSON specification.

*/
@interface SBJSON : NSObject
{
    BOOL humanReadable;
    BOOL sortKeys;
    NSUInteger maxDepth;

    @private
    // Used temporarily during scanning/generation
    NSUInteger depth;
    const char *c;
}

/// Whether we are generating human-readable (multiline) JSON
/**
 Set whether or not to generate human-readable JSON. The default is NO, which produces
 JSON without any whitespace. (Except inside strings.) If set to YES, generates human-readable
 JSON with linebreaks after each array value and dictionary key/value pair, indented two
 spaces per nesting level.
 */
#ifdef DARWIN
@property BOOL humanReadable;
#else
- (BOOL) humanReadable;
- (void) setHumanReadable:(BOOL)humanReadable;
#endif

/// Whether or not to sort the dictionary keys in the output
/** The default is to not sort the keys. */
#ifdef DARWIN
@property BOOL sortKeys;
#else
- (BOOL) sortKeys;
- (void) setSortKeys:(BOOL)sortKeys;
#endif

/// The maximum depth the parser will go to
/** Defaults to 512. */
#ifdef DARWIN
@property NSUInteger maxDepth;
#else
- (NSUInteger) maxDepth;
- (void) setMaxDepth:(NSUInteger)maxDepth;
#endif

/// Return JSON representation of an array  or dictionary
- (NSString*)stringWithObject:(id)value error:(NSError**)error;

/// Return JSON representation of any legal JSON value
- (NSString*)stringWithFragment:(id)value error:(NSError**)error;

/// Return the object represented by the given string
- (id)objectWithString:(NSString*)jsonrep error:(NSError**)error;

/// Return the fragment represented by the given string
- (id)fragmentWithString:(NSString*)jsonrep error:(NSError**)error;

/// Return JSON representation (or fragment) for the given object
- (NSString*)stringWithObject:(id)value
allowScalar:(BOOL)x
error:(NSError**)error;

/// Parse the string and return the represented object (or scalar)
- (id)objectWithString:(id)value
allowScalar:(BOOL)x
error:(NSError**)error;

@end

/// Adds JSON generation to NSObject subclasses
@interface NSObject (NSObject_SBJSON)

/**
 @brief Returns a string containing the receiver encoded as a JSON fragment.

 This method is added as a category on NSObject but is only actually
 supported for the following objects:
 @li NSDictionary
 @li NSArray
 @li NSString
 @li NSNumber (also used for booleans)
 @li NSNull
 */
- (NSString *)JSONFragment;

/**
 @brief Returns a string containing the receiver encoded in JSON.

 This method is added as a category on NSObject but is only actually
 supported for the following objects:
 @li NSDictionary
 @li NSArray
 */
- (NSString *)JSONRepresentation;

@end

/// Adds JSON parsing to NSString
@interface NSString (NSString_SBJSON)

/// Returns the object represented in the receiver, or nil on error.
- (id)JSONFragmentValue;

/// Returns the dictionary or array represented in the receiver, or nil on error.
- (id)JSONValue;

@end
