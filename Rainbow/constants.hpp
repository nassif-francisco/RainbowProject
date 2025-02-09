#ifndef RBCONSTANTS
#define RBCONSTANTS

#include <string>

static class RBConstants 

{
public: 
	static constexpr float windowHeight = 1080.f;
	static constexpr float toolbarHeight = 94.f;
	static constexpr float toolbarHeightFactor = 11.48;
	static constexpr float VerticesSize = 8.0f;
	


};

enum class RBActionState { NONE, PANNING, PAINTING, BRUSHING, MOVING, EDITING };


#endif /* ANIMATION_HANDLER_HPP */