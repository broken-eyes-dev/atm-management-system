# TODO

## Versions

- [x] v1.0.0 - MVP with all required features
- [x] v1.0.1 - Bug fixes and stability improvements
- [ ] v1.1.0 - Refactor and code quality improvements


## Fixes

- [x] Replace unsafe password return with credential validation
- [x] Load authenticated user ID correctly after login
- [x] Assign valid record IDs when creating accounts
- [x] Keep the main menu running after an operation returns
- [x] Prevent error messages from being cleared immediately when returning to the main menu
- [x] Validate account type input
- [x] Review phone number storage because leading zeros are lost
- [x] Review normal program exits using `exit(1)`
- [x] Improve file reading validation (`fscanf` conversion count, ==11 instead of !=EOF)


## Refactor

- [ ] Replace `goto` in `createNewAcc`
- [ ] Review duplicated record-writing logic
- [ ] Review helper functions and their responsibilities
- [ ] Clean up the Makefile


## Robustness / Error handling

- [ ] Check return values of `remove()` and `rename()` when replacing `records.txt`
- [ ] Review file operation failures to avoid leaving inconsistent temporary files


## Later / Polish

- [ ] Review inconsistent navigation and success/error flow
- [ ] Clean up UI messages
- [ ] Review comments
- [ ] Write or improve README