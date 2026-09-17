// Divides into multiple triangle, and sum up
const double PI=acos(-1);
double _area(pdd pa, pdd pb, double r){
  if(abs(pa)<abs(pb)) swap(pa, pb);
  if(abs(pb)<eps) return 0;
  double ar, h, theta;
  double a=abs(pb),b=abs(pa),c=abs(pb-pa);
  double cosB = dot(pb,pb-pa) / a / c, B = acos(cosB);
  double cosC = dot(pa,pb) / a / b, C = acos(cosC);
  if(a > r){
    ar = (C / 2) * r * r;
    h = a*b*sin(C)/c;
    if (h < r && B < PI / 2)
      ar -= (acos(h / r) * r * r -
        h * sqrt(r * r - h * h));
  }
  else if(b > r){
    theta = PI - B - asin(sin(B)/r*a);
    ar = .5 * a * r * sin(theta) +
      (C - theta) / 2 * r * r;
  } else ar = .5 * sin(C) * a * b;
  return ar;
}
double area_poly_circle(const vector<pdd> poly,const pdd &O,const double r){
  double ar = 0;
  FOR (i, 0, SZ(poly) - 1)
    ar += _area(poly[i] - O,
            poly[(i + 1) % SZ(poly)] - O, r) *
      ori(O, poly[i], poly[(i + 1) % SZ(poly)]);
  return fabs(ar);
}
