#include <iostream>
#include <sys/time.h>
#include "PBC.h"

using namespace std;

int main(int argc, char **argv)
{
  const char *paramFileName = (argc > 1) ? argv[1] : "pairing.param";
  FILE *sysParamFile = fopen(paramFileName, "r");
  if (sysParamFile == NULL) {
    cerr<<"Can't open the parameter file " << paramFileName << "\n";
    cerr<<"Usage: " << argv[0] << " [paramfile]\n";
    return 0;
  }
  Pairing e(sysParamFile);
  cout<<"Is symmetric? "<< e.isSymmetric()<< endl;
  cout<<"Is pairing present? "<< e.isPairingPresent()<< endl;  
  fclose(sysParamFile);

  G1 p(e,false);
  p = G1(e, "1234567", 7);
  p.dump(stdout,"Hash for 1234567 is ",10);
  G2 q(e,false);
  Zr r(e,(long int)10121);
  r.dump(stdout,"r",10);
  // Create a random element of Zr
  Zr s(e,true);
  s.dump(stdout,"s",10);
  r =s;
  r.dump(stdout,"new r",10);
  GT LHS = e(p,q)^r;
  G1 pr(p^r);
  GPP<G1> pp(p, e);
  GPP<G2> qp(q, e);
  GPP<GT> LHSp(LHS, e);
  struct timeval st, et;
  const int niter = 1000;
  gettimeofday(&st, NULL);
  G1 Q(e);
  for(int i=0;i<niter;++i) {
    Q *= (p^r);
  }
  gettimeofday(&et, NULL);
  unsigned long uselapsed = (et.tv_sec-st.tv_sec)*1000000 +
			      (et.tv_usec-st.tv_usec);
  gettimeofday(&st, NULL);
  G1 Qp(e);
  for(int i=0;i<niter;++i) {
    Qp *= (pp^r);
  }
  gettimeofday(&et, NULL);
  unsigned long uselapsed_pre = (et.tv_sec-st.tv_sec)*1000000 +
				  (et.tv_usec-st.tv_usec);
  gettimeofday(&st, NULL);
  G2 Q2(e);
  for(int i=0;i<niter;++i) {
    Q2 *= (q^r);
  }
  gettimeofday(&et, NULL);
  unsigned long uselapsed2 = (et.tv_sec-st.tv_sec)*1000000 +
			      (et.tv_usec-st.tv_usec);
  gettimeofday(&st, NULL);
  G2 Q2p(e);
  for(int i=0;i<niter;++i) {
    Q2p *= (qp^r);
  }
  gettimeofday(&et, NULL);
  unsigned long uselapsed2_pre = (et.tv_sec-st.tv_sec)*1000000 +
				  (et.tv_usec-st.tv_usec);
  gettimeofday(&st, NULL);
  GT QT(e);
  for(int i=0;i<niter;++i) {
    QT *= (LHS^r);
  }
  gettimeofday(&et, NULL);
  unsigned long uselapsedT = (et.tv_sec-st.tv_sec)*1000000 +
			      (et.tv_usec-st.tv_usec);
  gettimeofday(&st, NULL);
  GT QTp(e);
  for(int i=0;i<niter;++i) {
    QTp *= (LHSp^r);
  }
  gettimeofday(&et, NULL);
  unsigned long uselapsedT_pre = (et.tv_sec-st.tv_sec)*1000000 +
				  (et.tv_usec-st.tv_usec);
  G1 prpp(pp^r);
  p.dump(stdout,"p", 10);
  q.dump(stdout, "q", 10);
  pr.dump(stdout,"p^r (reg)", 10);
  prpp.dump(stdout,"p^r (pre)", 10);
  Q.dump(stdout,"Q (reg)", 10);
  Qp.dump(stdout,"Q (pre)", 10);
  Q2.dump(stdout,"Q2 (reg)", 10);
  Q2p.dump(stdout,"Q2 (pre)", 10);
  QT.dump(stdout,"QT (reg)", 10);
  QTp.dump(stdout,"QT (pre)", 10);
  cout << "time G1 (reg) = " << uselapsed << " us / " << niter << " = " <<
      uselapsed / niter << " us\n";
  cout << "time G1 (pre) = " << uselapsed_pre << " us / " << niter << " = " <<
      uselapsed_pre / niter << " us\n";
  cout << "time G2 (reg) = " << uselapsed2 << " us / " << niter << " = " <<
      uselapsed2 / niter << " us\n";
  cout << "time G2 (pre) = " << uselapsed2_pre << " us / " << niter << " = " <<
      uselapsed2_pre / niter << " us\n";
  cout << "time GT (reg) = " << uselapsedT << " us / " << niter << " = " <<
      uselapsedT / niter << " us\n";
  cout << "time GT (pre) = " << uselapsedT_pre << " us / " << niter << " = " <<
      uselapsedT_pre / niter << " us\n";
  GT RHS = e((p^r),q);
  LHS.dump(stdout,"LHS", 10);
  RHS.dump(stdout,"RHS", 10);

  if((e(p,q)^r) == e(p^r,q))
	cout<<"Correct Pairing Computation"<<endl;
  else
	cout<<"Incorrect Pairing Computation"<<endl;
  if((p.inverse()).square() == (p.square()).inverse())
	cout<<"Inverse, Square works"<<endl;
  else
	cout<<"Inverse, Square does not work."<<endl;
  G1 a;
  a = p;
  p.dump(stdout,"p is ") ;
  a.dump(stdout,"a is ") ;
  // Create the identity element b (in the same group as a)
  G1 b(a,true);
  b.dump(stdout,"b is ") ;
  return 0;
}
