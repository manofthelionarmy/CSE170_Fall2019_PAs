# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>

# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvAdd, EvInfo, EvExit };
   public :
	MyViewer ( int x, int y, int w, int h, const char* l );
	void add_ui ();
	void add_mynode ( int n );
	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
	void torus_node(float r, float R, int n); 
	GsVec torus_function(int phi, int theta, float r, float R); 
	void update_torus(float r, float R, int n); 
};
