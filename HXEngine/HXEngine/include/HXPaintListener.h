#pragma once
namespace HX3D
{
	class HXPaintListener
	{
	public:
		/*HXPaintListener() {}*/
		virtual ~HXPaintListener() {}

		//virtual void OnUpdate() = 0;
		virtual void OnPaint() = 0;
	};
}