//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Scene
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef Scene_h
#define Scene_h
//--------------------------------------------------------------------------------
#include "Node3D.h"
#include "Camera.h"
#include "ParameterContainer.h"
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Update( float time );
		virtual void Render( RendererDX11* Renderer );

		void AddCamera( Camera* camera );
		void AddActor( Actor* actor );

		// Geometric queries
		void BuildPickRecord( Ray3f& ray, std::vector<PickRecord>& record );

		Node3D* GetRoot();

	public:
		ParameterContainer Parameters;

	protected:
		Node3D* m_pRoot;
		std::vector< Camera* > m_vCameras;
		std::vector< Actor* > m_vActors;
	};
};
//--------------------------------------------------------------------------------
#endif // Scene_h
