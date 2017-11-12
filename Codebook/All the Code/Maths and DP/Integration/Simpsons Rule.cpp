/*
    numercial integration.
*/

const double eps = 1e-8;
const double PI = acos(-1);

double f (double x) {
    /* return value of function at value of x */
}

inline double simpson(double fl,double fr,double fmid,double l,double r) { return (fl+fr+4.0*fmid)*(r-l)/6.0; }

double rsimpson(double slr,double fl,double fr,double fmid,double l,double r) {
    double mid = (l+r)*0.5;
    double fml = f((l+mid)*0.5);
    double fmr = f((mid+r)*0.5);
    double slm = simpson(fl,fmid,fml,l,mid);
    double smr = simpson(fmid,fr,fmr,mid,r);
    if(fabs(slr-slm-smr) < eps) return slm+smr;
    return rsimpson(slm,fl,fmid,fml,l,mid)+rsimpson(smr,fmid,fr,fmr,mid,r);
}

double integrate(double l,double r) {
    double mid = (l+r)*.5;
    double fl = f(l);
    double fr = f(r);
    double fmid = f(mid);
    return rsimpson(simpson(fl,fr,fmid,l,r),fl,fr,fmid,l,r);
}