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
// UnorderedAccessViewDX11
//
//--------------------------------------------------------------------------------
#ifndef UnorderedAccessViewDX11_h
#define UnorderedAccessViewDX11_h
//--------------------------------------------------------------------------------
#include "ResourceViewDX11.h"
#include "PipelineManagerDX11.h"
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class UnorderedAccessViewDX11 : public ResourceViewDX11
	{
	public:
		UnorderedAccessViewDX11( ID3D11UnorderedAccessView* pView );
		virtual ~UnorderedAccessViewDX11();

	protected:
		ID3D11UnorderedAccessView*			m_pUnorderedAccessView;
		
		friend OutputMergerStageDX11;
		friend PipelineManagerDX11;
		friend RendererDX11;
	};
};
//--------------------------------------------------------------------------------
#endif // UnorderedAccessViewDX11_h
//--------------------------------------------------------------------------------

