#include <bits/stdc++.h>
#include "common.hpp"

using namespace std;

const int N_GROUPS = 3;

using Pics = vector<Picture>;

void splitPictures(const Pics &pics, Pics &vertical, Pics &horizontal)
{
    for (const auto &val : pics)
    {
        if (val.paired)
            vertical.push_back(val);
        else
            horizontal.push_back(val);
    }
}

vector<Slide> slides;
map <string, int> tags;
map <string, int> tagTypes;
vector <Picture> groups[N_GROUPS];

int countPicTypes(const Picture &pic)
{
    array<int, N_GROUPS> ret;

    for (int i = 0; i < N_GROUPS; i++)
      ret[i] = 0;

    int maxi = -1;
    int num;

    for (const auto &tag : pic.tags)
    {
        ret[tagTypes[tag]]++;
    }

    for (int i = 0; i < N_GROUPS; i++)
    {
        if (maxi < ret[i])
        {
            maxi = ret[i];
            num = i;
        }
    }

    return num;
}

vector <pair<Picture, Picture>> pairs;
vector <int> seti;
int wsk[N_GROUPS];

int main()
{
    srand(time(0));
    vector<Picture> pictures = readPictures();
    Pics hori, vert;
    splitPictures(pictures, vert, hori);

    for (const auto& pic : vert)
    {
        for (const auto& tag : pic.tags)
        {
            tags[tag]++;
        }
    }

    int value = 0;
    vector <string> tagVector;
    for (const auto& [tag, count] : tags)
    {
        tagVector.push_back(tag);
    }

    random_shuffle(tagVector.begin(), tagVector.end());
    for (const auto& tag : tagVector)
    {
        tagTypes[tag] = value % N_GROUPS;
        value++;
    }

    for (const auto& pic : vert)
    {
        int tmp = countPicTypes(pic);

        groups[tmp].push_back(pic);
    }

    for (int i = 0; i < N_GROUPS; i++)
      random_shuffle(groups[i].begin(), groups[i].end());

    //////////////////////////////////////////////////////////////

    for (int i = 0; i < N_GROUPS; i++)
    {
        if (groups[i].size() == 0)
          continue;
        cerr<<"CO "<<i<<endl;
        seti.push_back(i), wsk[i] = 0;
    }

    while (!seti.empty())
    {
        if (seti.size() == 1)
        {
            for (int i = wsk[seti[0]]; i < int(groups[seti[0]].size()); i += 2)
            {
                pairs.push_back({groups[seti[0]][i], groups[seti[0]][i + 1]});
            }

            break;
        }

        int a = rnd(0, seti.size() - 1);
        int b = rnd(0, seti.size() - 2);

        if (b == a)
          b = seti.size() - 1;

        pairs.push_back({groups[a][wsk[a]++], groups[b][wsk[b]++]});

        if (wsk[a] == int(groups[a].size()))
          seti.erase(find(seti.begin(), seti.end(), a));

        if (wsk[b] == int(groups[b].size()))
          seti.erase(find(seti.begin(), seti.end(), b));
    }

    cout<<pairs.size() + hori.size()<<endl;
    for (const auto &[first, second] : pairs)
    {
        cout<<first.id<<" "<<second.id<<endl;
    }

    for (const auto &val : hori)
    {
        cout<<val.id<<endl;
    }

    return 0;
}
