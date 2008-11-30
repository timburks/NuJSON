(load "JSON")

(class TestExamples is NuTestCase
     
     (- testJsonOrg is
        (set dir "test/json.org")
        (set files ((NSFileManager defaultManager) enumeratorAtPath:dir))
        (while (set file (files nextObject))
               (unless ((file pathExtension) isEqualToString:"json")
                       (continue))
               (set jsonPath (dir stringByAppendingPathComponent:file))
               (set plistPath ((jsonPath stringByDeletingPathExtension) stringByAppendingPathExtension:"plist"))
               (set jsonRep (NSString stringWithContentsOfFile:jsonPath
                                 encoding:NSASCIIStringEncoding
                                 error:nil))
               (set json (jsonRep JSONValue))
               (assert_true json)
               (assert_equal json (((json JSONRepresentation) JSONValue)))
               (set plist (NSString stringWithContentsOfFile:plistPath
                               encoding:NSASCIIStringEncoding
                               error:nil))
               (unless plist (continue))
               (set expected (plist propertyList))
               (assert_equal expected json)
               (assert_equal expected (((expected JSONRepresentation) JSONValue)))))
     
     (- testRFC4627 is
        (set dir "Tests/rfc4627")
        (set files ((NSFileManager defaultManager) enumeratorAtPath:dir))
        (while (set file (files nextObject))
               (unless ((file pathExtension) isEqualToString:"json")
                       (continue))
               (set jsonPath (dir stringByAppendingPathComponent:file))
               (set plistPath ((jsonPath stringByDeletingPathExtension) stringByAppendingPathExtension:"plist"))
               (set jsonRep (NSString stringWithContentsOfFile:jsonPath
                                 encoding:NSASCIIStringEncoding
                                 error:nil))
               (set json (jsonRep JSONValue))
               (assert_true json)
               (assert_equal json ((json JSONRepresentation) JSONValue))
               (set plist (NSString stringWithContentsOfFile:plistPath
                               encoding:NSASCIIStringEncoding
                               error:nil))
               (set expected (plist propertyList))
               (assert_equal expected json)
               (assert_equal expected (((expected JSONRepresentation) JSONValue)))))
     
     (- testJsonChecker is
        (set dir "test/jsonchecker")
        (set files ((NSFileManager defaultManager) enumeratorAtPath:dir))
        (set sbjson (SBJSON new))
        (sbjson setMaxDepth:19)
        (while (set file (files nextObject))
               (unless ((file pathExtension) isEqualToString:"json")
                       (continue))
               (set json (NSString stringWithContentsOfFile:(dir stringByAppendingPathComponent:file)
                              encoding:NSASCIIStringEncoding
                              error:nil))
               (if (file hasPrefix:"pass")
                   (then (assert_true (sbjson objectWithString:json error:nil)))
                   (else (assert_false (sbjson objectWithString:json error:nil)))))))