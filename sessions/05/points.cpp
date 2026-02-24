struct Point {
    float x, y, z;
};


int main() {
vector<Point> points;

  for (int i = 0; i < 3; i++) {
    points.push_back({i, i, i});
  }
  // send this to the server. WRite your own client.
  // simple: just print it out.
  // more awesome: server calculates the box that holds the points.

  // (minx, maxx, miny, maxy, minz, maxz)
return 0;
}