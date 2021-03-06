# lmsmap

This is an implementation of data structure that maps strings
to objects of arbitrary type. Conceptually you can think of
it as std::map<std::string, T>. However it has some special
design features:

- low memory usage
- cache friendliness
- insertions can be very slow

It is designed for cases where you have few updates but many
queries.

# Performance

The following numbers compare a std::map<std::string, int> to
lms::LmsMap<std::string, int>. The test data used is the standard
/usr/share/dict/words (with the possessive forms removed). The
platform is 64bit Ubuntu Vivid under VirtualBox.

Massif says std::map uses 5.6 MB of memory while lmsmap uses
2.6. I suspect that these numbers are a bit fishy, because
just printing out lmsmap's container sizes gives a smaller
number.

Query speed is ~900k q/s for std::map and 1800k q/s for lmsmap,
so lms is more than 2x as fast.

Insertion speed is 700k items/s for std::map and 35k items/s
for lmsmap. This is expected as middle insertions are very slow.
If insertions are done in sorted order std::map does 1200k items/s
but lmsmap does a whopping 3700k items/s.

# Contributing

For various reasons I need to keep the copyright on this code all
to myself. This means that in order to accept patches I would need
to do all the legal stuff of copyright assignments and all that.
I have no interest in that so therefore submissions from third parties
are not accepted. Sorry. Feel free to fork the code on your own, though.

# API stability

There is none.

Under **no** circumstances should you expose this in your public
headers.

# Contact

jpakkane at gmail dot com
