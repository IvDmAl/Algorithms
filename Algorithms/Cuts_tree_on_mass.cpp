#include <vector>

using namespace std;

template<class T>
class CutsTree {
public:
  CutsTree(const vector<T>& a, T(*f)(const T&, const T&))
  {
    f_ = f;
    len_mass_ = a.size();
    n_lists_ = (1 << int(ceil(log2(a.size()))));
    tree_.resize(2 * n_lists_);
    //adds_vals_.resize(2 * n_lists_);
    build_(a, 1, 0, n_lists_ - 1);
  }

  T calc(int l, int r) {
    return calc_(1, 0, n_lists_ - 1, l, r).first;
  }

  void update_elem(int ind, const T& val) {
    update_elem_(1, 0, n_lists_ - 1, ind, val);
  }
private:
  void update_node_(int v) {
    if (!tree_[lv(v)].second && !tree_[rv(v)].second)
      tree_[v] = { T(), false };
    if (!tree_[lv(v)].second && tree_[rv(v)].second)
      tree_[v] = tree_[rv(v)];
    if (tree_[lv(v)].second && !tree_[rv(v)].second)
      tree_[v] = tree_[lv(v)];
    if (tree_[lv(v)].second && tree_[rv(v)].second)
      tree_[v] = { f_(tree_[lv(v)].first, tree_[rv(v)].first), true };
  }

  void build_(const vector<T>& a, int v, int tl, int tr)
  {
    if (tl == tr)
      if (tl >= a.size())
        tree_[v] = { T(), false };
      else
        tree_[v] = { a[tl], true };
    else
    {
      int tm = (tl + tr) / 2;
      build_(a, lv(v), tl, tm);
      build_(a, rv(v), tm + 1, tr);
      update_node_(v);
    }
  }

  pair<T, bool> calc_(int v, int tl, int tr, int l, int r) {
    if (l > r || l >= len_mass_)
      return { T(), false };
    if (l == tl && r == tr)
      return tree_[v];

    int tm = (tl + tr) / 2;
    pair<T, bool> al = calc_(lv(v), tl, tm, l, min(r, tm));
    pair<T, bool> ar = calc_(rv(v), tm + 1, tr, max(l, tm + 1), r);

    if (!al.second && !ar.second)
      return { T(), false };
    if (!al.second && ar.second)
      return ar;
    if (al.second && !ar.second)
      return al;
    if (al.second && ar.second)
      return { f_(al.first, ar.first), true };
  }

  void update_elem_(int v, int tl, int tr, int pos, const T& new_val) {
    if (tl == tr)
      tree_[v] = { new_val, true };
    else
    {
      int tm = (tr + tl) / 2;

      if (pos <= tm)
        update_elem_(lv(v), tl, tm, pos, new_val);
      else
        update_elem_(rv(v), tm + 1, tr, pos, new_val);

      update_node_(v);
    }
  }

  int lv(int ind) const { return ind * 2; }
  int rv(int ind) const { return ind * 2 + 1; }
  int n_lists_;
  int len_mass_;
  T(*f_)(const T&, const T&);
  vector<pair<T, bool>> tree_;
  //vector<T> adds_vals_;
};