# AudioFile testing

DI NOTES:
https://chat.openai.com/c/dc270032-ff73-4349-be3f-739eeffd45ad

Use this to coordinate test dependencies
``` cpp
bool firstTestPassed = false;

TEST(TestSuite, FirstTest) {
    // Your test logic here
    firstTestPassed = /* your condition */;
}

TEST(TestSuite, DependentTest) {
    if (!firstTestPassed) {
        GTEST_SKIP();
        return;
    }
    // Your test logic here
}
```

## Notes:
- all tests should fully set up and clean up all state
- Use global state technique to determine if tests should be skipped


## Unit tests:
- AudioFileRepository
    - Write
        - all valid combos write
        - invalid combos throw
        - missing file throws
    - Read
        - depends on Write
        - written file can be read in, is identical*
        -   \* must be written as 32-float-wav for this to be the case 
    - Append
        - depends on Write, Read
        - Write then append, read to validate
        - appended file can be read in, is identical* x 2
        -   \* must be written as 32-float-wav for this to be the case
        - append to missing file throws (catch read exception, re throw)
        - append to invalid file throws
    - Delete
        - depends on Write
        - write, then delete
        - delete non-audio file throws
        - delete non-existing file throws

- WriteSpecValidator
    - missing path throws
    - valid combos don't throw
        ``` cpp
        EXPECT_NO_THROW({
            // Your code that should not throw an exception
        });

        ```
    - invalid combos throw
    - WriteSettingsFake (or something) to test the not implemented path

- AudioFileWriteSettings
    - WriteSettingsPcm, WriteSettingsMp3 static builder methods construct correctly

## integration
- AudioFileService
    - should I use google mock?
        - https://chat.openai.com/c/62db90fa-fcab-4df4-a683-4c1c6f2bad59
    - if so, ignore actually checking the results, just clean up.
    - write one, write many works.
    - read one, read many[1] works
    - append one, append many[1] works
    - delete one, delete many[1] works

    