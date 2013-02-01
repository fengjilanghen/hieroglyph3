//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------
#include "App.h"
#include "Log.h"

#include <sstream>

#include "EventManager.h"
#include "EvtFrameStart.h"
#include "EvtKeyUp.h"
#include "EvtKeyDown.h"

#include "GeometryGeneratorDX11.h"
#include "MaterialGeneratorDX11.h"
#include "RotationController.h"

#include "VectorParameterDX11.h"
#include "BasicVertexDX11.h"

using namespace Glyph3;
//--------------------------------------------------------------------------------
App AppInstance; // Provides an instance of the application
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
App::App()
{
}
//--------------------------------------------------------------------------------
bool App::ConfigureEngineComponents()
{
	if ( !ConfigureRenderingEngineComponents( 800, 600, D3D_FEATURE_LEVEL_11_0 ) ) {
		return( false );
	}

	if ( !ConfigureRenderingSetup() ) {
		return( false );
	}

	SetMultiThreadedMode( false );

	return( true );
}
//--------------------------------------------------------------------------------
void App::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void App::Initialize()
{
	// Create the camera, and the render view that will produce an image of the 
	// from the camera's point of view of the scene.

	m_pCamera->Spatial().SetRotation( Vector3f( 0.5f, 0.3f, 0.0f ) );
	m_pCamera->Spatial().SetTranslation( Vector3f( -3.0f, 12.0f, -15.0f ) );
	m_pRenderView->SetBackColor( Vector4f( 0.2f, 0.2f, 0.4f, 0.0f ) );

	// Create the actor to hold the immediate geometry.  

	m_pIndexedActor = new GeometryActor();
	m_pScene->AddActor( m_pIndexedActor );
	m_pIndexedActor->GetNode()->Position() = Vector3f( 3.0, 0.0f, 0.0f );
	m_pIndexedActor->SetColor( Vector4f( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_pIndexedActor->UseTexturedMaterial( m_pRenderer11->LoadTexture( L"EyeOfHorus_128.png" ) );

	// Throw a rotation onto the actor to slowly rotate it about the Y-axis.
	RotationController* pIndexedRotController = new RotationController( Vector3f( 0.0f, 1.0f, 0.0f ), -0.1f );
	m_pIndexedActor->GetNode()->AttachController( pIndexedRotController );



	m_pGeometryActor = new GeometryActor();
	m_pScene->AddActor( m_pGeometryActor );
	m_pGeometryActor->GetNode()->Position() = Vector3f( 0.0f, 2.5f, 0.0f );

	m_pGeometryActor->SetColor( Vector4f( 1.0f, 0.0f, 0.0f, 1.0f ) );
	m_pGeometryActor->DrawSphere( Vector3f( 2.5f, 2.0f, 0.0f ), 1.5f, 16, 24 );
	m_pGeometryActor->SetColor( Vector4f( 0.0f, 1.0f, 0.0f, 1.0f ) );
	m_pGeometryActor->DrawCylinder( Vector3f( -1.5f, -1.0f, 0.0f ), Vector3f( -1.5f, 3.0f, 0.0f ), 1.5f, 0.0f, 8, 24 );
	m_pGeometryActor->SetColor( Vector4f( 1.0f, 1.0f, 0.0f, 1.0f ) );
	m_pGeometryActor->DrawDisc( Vector3f( 0.0f, -3.0f, 0.0f ), Vector3f( 1.0f, 1.0f, 1.0f ), 2.0f, 12 );
	m_pGeometryActor->SetColor( Vector4f( 0.0f, 0.0f, 1.0f, 1.0f ) );
	m_pGeometryActor->DrawBox( Vector3f( 0.0f, 3.0f, 0.0f ), Vector3f( 1.0f, 1.0f, 1.0f ) );

	RotationController* pGeometryRotController = new RotationController( Vector3f( 0.0f, 1.0f, 0.0f ), 0.4f );
	m_pGeometryActor->GetNode()->AttachController( pGeometryRotController );


	m_pTextActor = new TextActor();
	m_pTextActor->SetTextOrientation( Vector3f( 1.0f, 0.0f, 0.0f ), Vector3f( 0.0f, 1.0f, 0.0f ) );
	m_pTextActor->SetText( std::wstring( L"Hello World! This is some \nsample text!" ) );
	
	m_pTextActor->SetColor( Vector4f( 0.0f, 1.0f, 0.0f, 0.5f ) );
	m_pTextActor->NewLine();
	m_pTextActor->SetCharacterHeight( 2.0f );
	m_pTextActor->AppendText( L"Text is colored, scaled, and alpha blended..." );

	m_pTextActor->GetNode()->Position() = Vector3f( 0.0f, 7.0f, 0.0f );
	m_pScene->AddActor( m_pTextActor );


	// Do a one time initialization of the scene lighting parameters here
	Vector4f Ka( 0.2f, 0.2f, 0.2f, 0.2f );
	VectorParameterDX11* FactorKa = m_pRenderer11->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Ka" ) );
	FactorKa->InitializeParameterData( &Ka );

	Vector4f Kd( 0.5f, 0.5f, 0.5f, 0.5f );
	VectorParameterDX11* FactorKd = m_pRenderer11->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Kd" ) );
	FactorKd->InitializeParameterData( &Kd );

	Vector4f Ks( 1.0f, 1.0f, 1.0f, 1.0f );
	VectorParameterDX11* FactorKs = m_pRenderer11->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Ks" ) );
	FactorKs->InitializeParameterData( &Ks );

	Vector4f LightPosition( 100.0f, 100.0f, -100.0f, 0.0f );
	VectorParameterDX11* FactorLightPosition = m_pRenderer11->m_pParamMgr->GetVectorParameterRef( std::wstring( L"LightPosition" ) );
	FactorLightPosition->InitializeParameterData( &LightPosition );

	Vector4f Ia( 0.25f, 0.25f, 0.25f, 0.25f );
	VectorParameterDX11* FactorIa = m_pRenderer11->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Ia" ) );
	FactorIa->InitializeParameterData( &Ia );

	Vector4f Id( 0.5f, 0.5f, 0.5f, 1.0f );
	VectorParameterDX11* FactorId = m_pRenderer11->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Id" ) );
	FactorId->InitializeParameterData( &Id );

	Vector4f Is( 1.0f, 1.0f, 1.0f, 1.0f );
	VectorParameterDX11* FactorIs = m_pRenderer11->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Is" ) );
	FactorIs->InitializeParameterData( &Is );
}
//--------------------------------------------------------------------------------
void App::Update()
{
	// Update the timer to determine the elapsed time since last frame.  This can 
	// then used for animation during the frame.

	m_pTimer->Update();

	EvtManager.ProcessEvent( EvtFrameStartPtr( new EvtFrameStart( m_pTimer->Elapsed() ) ) );


	// Do some immediate rendering here.  These vertex data are applied in object
	// space, so we are essentially redefining our geometry in every frame.  This
	// is normally not the most efficient way, but it is significantly easier to
	// dynamically produce than it is to determine before hand what you need...
	//
	// In this case, I am animating a paraboloid shape as well as its color.
	
	const int GRIDSIZE = 20;
	const float fGRIDSIZE = static_cast<float>( GRIDSIZE );
	const float ELEMSIZE = 1.0f;
	const float fSIZESCALE = 5.0f / fGRIDSIZE;

	Vector3f v0, v1, v2;
	Vector2f uv0, uv1, uv2;
	Vector3f n0, n1, n2;

	float fScaling = 0.25f * sinf( m_pTimer->Runtime() * 0.75f );

	// Do some indexed immediate rendering by creating a grid of vertices first...

	m_pIndexedActor->ResetGeometry();

	for ( int z = 0; z < GRIDSIZE; z++ ) {
		for ( int x = 0; x < GRIDSIZE; x++ ) {
			
			float fX = static_cast<float>( x );
			float fZ = static_cast<float>( z );

			v0.x = fX - GRIDSIZE / 2;
			v0.z = fZ - GRIDSIZE / 2;
			v0.y = ( 5.0f - 0.2f * ( v0.x*v0.x + v0.z*v0.z ) ) * fScaling;
			uv0.x =        ( fX ) / ( fGRIDSIZE-1 );
			uv0.y = 1.0f - ( fZ ) / ( fGRIDSIZE-1 );

			m_pIndexedActor->AddVertex( v0 * fSIZESCALE, uv0 );
		}
	}

	// ... followed by creating the associated group of indices that reference
	// those vertices.

	for ( int z = 0; z < GRIDSIZE-1; z++ ) {
		for ( int x = 0; x < GRIDSIZE-1; x++ ) {

			m_pIndexedActor->AddIndex( (z*GRIDSIZE + x) );
			m_pIndexedActor->AddIndex( (z*GRIDSIZE + x) + GRIDSIZE );
			m_pIndexedActor->AddIndex( (z*GRIDSIZE + x) + 1 );

			m_pIndexedActor->AddIndex( (z*GRIDSIZE + x) + 1 );
			m_pIndexedActor->AddIndex( (z*GRIDSIZE + x) + GRIDSIZE );
			m_pIndexedActor->AddIndex( (z*GRIDSIZE + x) + GRIDSIZE + 1 );
		}
	}


	// Print a message to show the framerate and sample name.

	std::wstringstream out;

	out << L"Hieroglyph 3 : " << GetName() << std::endl;
	out << L"FPS: " << m_pTimer->Framerate();
	
	m_pTextOverlayView->WriteText( out.str(), Matrix4f::TranslationMatrix( 5.0f, 5.0f, 0.0f ),
		Vector4f( 1.0f, 1.0f, 1.0f, 1.0f ) );

	
	// Update the scene, and then render all cameras within the scene.

	m_pScene->Update( m_pTimer->Elapsed() );
	m_pScene->Render( m_pRenderer11 );


	// Perform the rendering and presentation for the window.

	m_pRenderer11->Present( m_pWindow->GetHandle(), m_pWindow->GetSwapChain() );

}
//--------------------------------------------------------------------------------
void App::Shutdown()
{
	// Print the framerate out for the log before shutting down.

	std::wstringstream out;
	out << L"Max FPS: " << m_pTimer->MaxFramerate();
	Log::Get().Write( out.str() );
}
//--------------------------------------------------------------------------------
bool App::HandleEvent( EventPtr pEvent )
{
	eEVENT e = pEvent->GetEventType();

	if ( e == SYSTEM_KEYBOARD_KEYDOWN )
	{
		EvtKeyDownPtr pKeyDown = std::static_pointer_cast<EvtKeyDown>( pEvent );

		unsigned int key = pKeyDown->GetCharacterCode();
	}
	else if ( e == SYSTEM_KEYBOARD_KEYUP )
	{
		EvtKeyUpPtr pKeyUp = std::static_pointer_cast<EvtKeyUp>( pEvent );

		unsigned int key = pKeyUp->GetCharacterCode();
	}

	// Call the parent class's event handler if we haven't handled the event.

	return( RenderApplication::HandleEvent( pEvent ) );
}
//--------------------------------------------------------------------------------
std::wstring App::GetName( )
{
	return( std::wstring( L"ImmediateRenderer" ) );
}
//--------------------------------------------------------------------------------