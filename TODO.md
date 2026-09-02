# TODO

## Versions

- [x] v1.0.0 - MVP with all required features
- [ ] v1.0.1 - Bug fixes and stability improvements
- [ ] v1.1.0 - Refactor and code quality improvements


## Fixes

- [x] Replace unsafe password return with credential validation
- [x] Load authenticated user ID correctly after login
- [x] Assign valid record IDs when creating accounts
- [x] Keep the main menu running after an operation returns
- [ ] Prevent error messages from being cleared immediately when returning to the main menu
- [ ] Validate account type input
- [ ] Review phone number storage because leading zeros are lost
- [ ] Review normal program exits using `exit(1)`
- [ ] Improve file reading validation (`fscanf` conversion count)


## Refactor

- [ ] Replace `goto` in `createNewAcc`
- [ ] Review duplicated record-writing logic
- [ ] Review helper functions and their responsibilities
- [ ] Clean up the Makefile


## Later / Polish

- [ ] Review inconsistent navigation and success/error flow
- [ ] Clean up UI messages
- [ ] Review comments
- [ ] Write or improve README