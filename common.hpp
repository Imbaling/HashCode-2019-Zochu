#include <bits/stdc++.h>

using namespace std;

using ull = unsigned long long;

class Picture {
public:
  int id;
  bool paired;
  set<string> tags;
};

Picture guard;

vector<Picture> input;

const int MAX_TAG = 12;

Picture readPicture()
{
    int m;
    char s[MAX_TAG];
    Picture pic;
    char type;

    int ignore = scanf(" %c %d", &type, &m);
    pic.paired = (type == 'V');

    for (int i = 0; i < m; i++)
    {
        ignore = scanf("%s", s);

        pic.tags.insert(s);
    }

    (void)ignore;

    return pic;
}

vector<Picture> readPictures()
{
    int n;
    vector<Picture> pics;

    cin>>n;

    for (int i = 0; i < n; i++)
    {
        pics.push_back(readPicture());
        pics[i].id = i;
    }

    return pics;
}

class Slide {
public:
  Slide(Picture &a): first(a), second(guard) {
    assert(a.paired == false);

    tags = a.tags;
  }

  Slide(Picture &a, Picture &b): first(a), second(b) {
    assert(a.paired);
    assert(b.paired);

    tags = a.tags;
    for (string tag: b.tags)
      tags.insert(tag);
  }

  Picture &first;
  Picture &second;

  set<string> tags;

  ull operator+(Slide &b) const {
    ull inter = 0;
    for (string tag: tags) {
      if (b.tags.find(tag) != b.tags.end())
        inter++;
    }

    ull diff1 = 0;
    for (string tag: tags) {
      if (b.tags.find(tag) == b.tags.end())
        diff1++;
    }

    ull diff2 = 0;
    for (string tag: b.tags) {
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
    ull ret = 0;
    for (int i = 0; i < (int)show.size() - 1; i++) {
      ret += show[i] + show[i+1];
    }

    return ret;
  }

  void print() {
    cout << show.size() << endl;
    for (int i = 0; i < (int)show.size(); i++) {
      if (show[i].first.paired) {
        cout << show[i].first.id << " " << show[i].second.id << endl;
      } else {
        cout << show[i].first.id << endl;
      }
    }
  }
};
