#include <bits/stdc++.h>

using namespace std;

using ull = unsigned long long;

class Picture {
public:
  char type;
  bool paired;
  set<string> tags;
};

Picture guard;

vector<Picture> input;

class Slide {
public:
  Slide(Picture &a) first(a), second(guard) {
    assert(a.paired == false);

    tags = a.tags;
  }

  Slide(Picture &a, Picture &b) first(a), second(b) {
    assert(a.paired);
    assert(b.paired);

    tags = a.tags;
    for (string &tag: b.tags)
      tags.insert(tag);
  }

  Picture &first;
  Picture &second;

  set<string> tags;

  ull operator+(Slide &b) const {
    ull inter = 0;
    for (string &tag: tags) {
      if (b.tags.find(tag) != b.tags.end())
        inter++;
    }

    ull diff1 = 0;
    for (string &tag: tags) {
      if (b.tags.find(tag) == b.tags.end())
        diff1++;
    }

    ull diff2 = 0;
    for (string &tag: b.tags) {
      if (tags.find(tag) == b.tags.end())
        diff2++;
    }

    return min(inter, min(diff1, diff2));
  }
};

class Slideshow {
public:
  vector<Slide> show;

  ull evaluate() {

  }
};
