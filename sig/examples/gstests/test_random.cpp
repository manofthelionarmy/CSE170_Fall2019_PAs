/*=======================================================================
   Copyright (c) 2018-2019 Marcelo Kallmann.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/gs_array.h>
# include <sig/gs_random.h>

// this test runs untill a 0 and a 1 are generated by gs_random()
static void test_01 ()
 {
   bool zero=false;
   bool one=false;
   int count=0;
   while (++count)
	{ double f = gs_random(); 
	  if ( f>0.999999 ) gsout.putf("%15.12f\n",f);
	  if ( f==1.0 ) { gsout<<"====================== 1 ========\n"; one=true; }
	  if ( f==0.0 ) { gsout<<"====================== 0 ========\n"; zero=true; }
	  if ( one && zero ) { gsout<<"Got both!\n"; gsout<<"Iterations:"<<count<<gsnl; gsout.exit(); }
	}
 }

static void test_i ( int a, int b, int times )
 {
   GsRandom<int> r ( a, b );
   GsArray<int> v;
   int i, n;

   v.size ( (b-a)+1 );
   v.setall ( 0 );

   gsout << times <<" random calls between "<<a<<" and "<<b<<"...";
   for ( i=0; i<times; i++ )
	{ n = r.get();
	  if ( n<a || n>b ) gsout.fatal ( "Int error: %d", n );
	  v[n-a]++;
	}

   gsout << " Ok.\n";
   gsout << "Distribution:\n";
   gsout << v << gsnl;
   gsout << gsnl;
 }

static void test_r ( float a, float b, int times )
 {
   GsRandom<float> rf ( a, b );
   GsRandom<double> rd ( a, b );
   GsArray<int> vf, vd;
   double d;
   float f;

   vf.size ( gs_round(b-a)+1 ); vf.setall ( 0 );
   vd.size ( gs_round(b-a)+1 ); vd.setall ( 0 );

   gsout << times <<" calls between "<<a<<" and "<<b<<"...";
   for ( int i=0; i<times; i++ )
	{ f = rf.get();
	  d = rd.get();
	  if ( f<a || f>b ) gsout.fatal ( "Float error: %f", f );
	  if ( d<a || d>b ) gsout.fatal ( "Double error: %d", d );
	  vf[int(f-a)]++;
	  vd[int(d-a)]++;
	}

   gsout << " Ok.\n";
   gsout << "Distribution:\n";
   gsout << vf << gsnl;
   gsout << vd << gsnl;
   gsout << gsnl;
 }

void test_random ()
 {
   double t0 = gs_time();

   test_i ( 2,  4, 300000 );
   test_i ( 1, 10, 100000 );
   test_i ( 6, 13, 800000 );

   test_r ( 0,	 1.05f, 500000 );
   test_r ( 0,	 5.5f,  500000 );
   test_r ( 10.1f, 20,	500000 );

   gsout<<"\nTime taken: "<<(gs_time()-t0)<<gsnl;


   int i, s=20;
   gsuint rs = 198312;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;

   rs = 987;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;

   rs = 198312;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;
   gs_rseed ( rs );
   for ( i=0; i<s; i++ ) { gsout<<gs_random(0,9)<<gspc; } gsout<<gsnl;

   //test_01 ();
 }
