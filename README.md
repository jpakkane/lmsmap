# lmsmap

This is an implementation of data structure that maps strings
to objects of arbitrary type. Conceptually you can think of
it as std::map<std::string, T>. However it has some special
design features:

- low memory usage
- insertions can be very slow

It is designed for cases where you build the structure
once and query it often.