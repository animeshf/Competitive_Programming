/*
    given any polygons, find the area of their union.
*/
inline double area(double a, double b, const vector<vector<PT> >& v) {
    vector<pair<double, int> > event;
    int cnt = 0;
    FOR(i,0,SZ(v)) {
        FOR(k,0,SZ(v[i])) {
            PT p1 = v[i][k];
            PT p2 = v[i][(k + 1) % SZ(v[i])];
            if (p1.X > p2.X) swap(p1, p2);
            if (p1.X <= a && p2.X >= b) {
                PT q1, q2;
                lineline(p1, p2, PT(a,0), PT(a,100), q1);
                lineline(p1, p2, PT(b,0), PT(b,100), q2);
                double yy = ((q1.Y + q2.Y) / 2);
                event.push_back((MP(yy, i)));
            }
        }
    }
    sort(event.begin(), event.end());
    double ans = 0, amnt = 0;
    set<int> met;
    FOR(i,0,SZ(event)) {
        double yy = event[i].A;
        int t = event[i].B;
        if (met.count(t)) {
            amnt --;
            if (amnt == 0)
                ans += yy;
            met.erase(t);
        } else {
            if (amnt == 0)
                ans -= yy;
            amnt ++;
            met.insert(t);
        }
    }
    return ans * (b - a);
}

double polygon_union_area(const vector<vector<PT> >& v) {
    int N = SZ(v);
    vector<double> xs;
    vector<pair<PT, PT> > all;
    FOR(i,0,N) {
        FOR(k,0,SZ(v[i])) {
            int j = (k + 1) % SZ(v[i]);
            all.push_back(MP(v[i][k], v[i][j]));
        }
    }
    FOR(i,0,SZ(all)) {
        xs.push_back(all[i].A.X);
        FOR(k,0,i) {
            PT p;
            if (lineline(all[i].A, all[i].B, all[k].A, all[k].B, p)) {
                if (p.X >= 0 && p.X <= 1000 && p.Y <= 1000 && p.Y >= 0)
                    xs.push_back(p.X);
            }
        }
    }
    double ans = 0;
    sort(xs.begin(), xs.end());
    for (int i = 0; i < xs.size() - 1; i++) {
        if (xs[i + 1] - xs[i] < eps) continue;
        ans += area(xs[i], xs[i + 1], v);
    }
    return ans;
}