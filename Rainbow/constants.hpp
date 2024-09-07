#ifndef RBCONSTANTS
#define RBCONSTANTS

#include <string>

static class RBConstants 

{
public: 
	static constexpr float windowHeight = 600.f;
	static constexpr float toolbarHeight = 100.f;


};

enum class RBActionState { NONE, PANNING, PAINTING, BRUSHING };


#endif /* ANIMATION_HANDLER_HPP */