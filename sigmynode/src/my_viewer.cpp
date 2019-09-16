
# include "my_viewer.h"
# include "sn_mynode.h"

#include <vector>
# include <sigogl/gl_tools.h>
# include <sigogl/ui_button.h>
# include <sig/gs_model.h>
# include <sig/sn_model.h>
# include <sig/sn_transform.h>


SnMyNode* c = new SnMyNode(); 
int numTriangles;
float littleR;
float bigR;


GsVec lighting = GsVec(0.015f, 0.15f, 0.15f);
GsMat shadowXY = GsMat(1.0f, 0.0f, -lighting.x / lighting.z, 0.0f,
	0.0f, 1.0f, -lighting.y / lighting.z, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);




MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	add_ui ();
	/**
	Default code given
	add_mynode (4);
	*/

	littleR = 0.1f;
	bigR = 0.5f;
	numTriangles = 10;

	// torus_node(littleR, bigR, numTriangles); 

	draw_clock();
}

void MyViewer::add_ui ()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "Add", EvAdd ) );
	p->add ( new UiButton ( "Info", EvInfo ) );
	p->add ( new UiButton ( "Exit", EvExit ) );
}

void MyViewer::add_mynode ( int n )
{
	SnMyNode* c;

	float r=0.15f; // position range
	while ( n-->0 )
	{	c = new SnMyNode;
		c->init.set ( gs_random(-r,r), gs_random(-r,r), gs_random(-r,r) );
		c->width = gs_random(0.001f,r);
		c->height = gs_random(0.001f,r*2);
		c->color ( GsColor::random() );
		// Example how to print/debug your generated data:
		// gsout<<n<<": "<<c->color()<<gsnl;
		rootg()->add(c);
	}
}

GsMat MyViewer::roty(float alpha) {
	float a = cosf(GS_TORAD(alpha));
	float b = sinf(GS_TORAD(alpha)); 

	return GsMat(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,    a,   -b, 0.0f,
		0.0f,    b,    a, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	); 
}

void MyViewer::draw_clock() {
	// root
	SnGroup* clock = new SnGroup;

	//Transforms
	SnTransform* shadow_transform = new SnTransform();
	SnTransform* rotate = new SnTransform();
	
	// 1st node
	SnGroup* body = new SnGroup;

	// 2nd node
	SnGroup* minute_hand = new SnGroup;

	// 3rd node
	SnGroup* hour_hand = new SnGroup;

	//clock body
	GsModel* c_b = new GsModel; 
	c_b->make_cylinder(GsPnt(0, 0, 0), GsPnt(0, 0, 0.1f), 0.3f, 0.3f, 25, true);

	body->add(new SnModel(c_b));
	body->top<SnModel>()->color(GsColor::blue);

	GsModel* hh = new GsModel; 
	hh->make_capsule(GsPnt(0.0f, 0.0f, 0.0f), GsPnt(0, 0.1f, 0), 0.03f, 0.03f, 25, true);

	hour_hand->add(new SnModel(hh)); 
	hour_hand->top<SnModel>()->color(GsColor::green); 

	hour_hand->separator(true);

	shadow_transform->set(shadowXY); 
	rotate->set(roty(90.0f)); 

	GsModel* mh = new GsModel;
	mh->make_capsule(GsPnt(0, 0, 0), GsPnt(0, 0.25f, 0), 0.03f, 0.03f, 25, true); 
	minute_hand->add(new SnModel(mh));
	minute_hand->top<SnModel>()->color(GsColor::red);

	minute_hand->separator(true);
	
	// make sure the transform is added before the body!!
	// think of it like a stack!! we want to pop off the GsModel first
	// then apply the transformation
	clock->add((SnNode*)(rotate)); 
	clock->add((SnNode*)(shadow_transform));
	clock->add_group(body);
	clock->add_group(hour_hand); 
	clock->add_group(minute_hand);

	rootg()->add_group(clock);

	return; 
}

GsVec MyViewer::torus_function(int phi, int theta, float r, float R) {

	float x = 0.0f, y = 0.0f, z = 0.0f; 
	float alpha = GS_TORAD(float(phi));
	float beta = GS_TORAD(float(theta));

	x = float(R + r * cosf(alpha)) * cosf(beta);
	y = float(R + r * cosf(alpha)) * sinf(beta);
	z = float(r * sinf(alpha));

	return GsVec(x, y, z);

}


void MyViewer::torus_node(float r, float R, int n, bool update) {

	// SnMyNode* c = new SnMyNode();
	
	int prevPhi = 0; 
	int prevTheta = 0; 

	int nextPhi = 0;
	int nextTheta = 0;

	int i = 0; 

	if (update) {
		// clear all the faces and redraw
		c->faces.clear(); 
	}

	for (nextPhi = n; prevPhi <= 360; nextPhi += n) {
		for (nextTheta = n; nextTheta <= 360; nextTheta += n) {

			SnMyNode::Face f;

			f.A00 = torus_function(prevPhi, prevTheta, r, R);
			f.A10 = torus_function(nextPhi, prevTheta, r, R);
			f.A01 = torus_function(prevPhi, nextTheta, r, R);
			f.A11 = torus_function(nextPhi, nextTheta, r, R);


			c->faces.push_back(f);
			c->color(GsColor::blue);
	

			prevTheta = nextTheta;
			
		}

		prevPhi = nextPhi;
	}

	
	update == true ? c->touch() : rootg()->add(c); 

	return; 
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case GsEvent::KeyLeft: gsout<<"Left\n"; return 1;

		case 'a':
		{
			
			++numTriangles; 

			torus_node(littleR, bigR, numTriangles, true); 

			render();

			return 1; 
		}
		case 'q': {

			numTriangles > 10 ? --numTriangles: numTriangles = 10;

			torus_node(littleR, bigR, numTriangles, true);

			render();

			return 1;
		}

		case 'w': {

			littleR += 0.1f; 

			torus_node(littleR, bigR, numTriangles, true);

			render();

			return 1; 
		}
		case 's': {
			
			littleR - 0.1f  >= 0.1f ? littleR -= 0.1f : littleR = 0.1f; 

			torus_node(littleR, bigR, numTriangles, true);

			render();

			return 1; 
		}
		case 'e': {
			bigR += 0.1f; 

			torus_node(littleR, bigR, numTriangles, true);

			render();

			return 1;
		}
		case 'd': {
			
			bigR - 0.1f >= 0.1f ? bigR -= 0.1f : bigR = 0.1f; 

			torus_node(littleR, bigR, numTriangles, true);

			render();

			return 1;

		}
		// etc
		default: gsout<<"Key pressed: "<<e.key<<gsnl;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvAdd: add_mynode(1); return 1;

		case EvInfo:
		{	if ( output().len()>0 ) { output(""); return 1; }
			output_pos(0,30);
			activate_ogl_context(); // we need an active context
			GsOutput o; o.init(output()); gl_print_info(&o); // print info to viewer
			return 1;
		}

		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
