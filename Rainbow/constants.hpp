#ifndef RBCONSTANTS
#define RBCONSTANTS

#include <string>
using namespace std;

static class RBConstants 

{
public: 
	static constexpr float windowHeight = 1080.f;
	static constexpr float toolbarHeight = 94.f;
	static constexpr float toolbarHeightFactor_1920x1080 = 11.48;
	static constexpr float toolbarHeightFactor_2560x1600 = 16;
	static constexpr float toolbarHeightFactor_1440x900 = 9.57;
	static constexpr float VerticesSize = 2.0f;
	static constexpr float InitialMapOffset = 1000.0f;
	inline static const string CommonMediaPath = "../../CommonMedia/";
	inline static const string CommonMediaEnvironmentPath = "../../CommonMedia/Environment/" ;
	inline static const string CommonMediaEnvironmentPacksPath = "../../CommonMedia/Environment/Packs/";
	inline static const string CommonMediaMapsPath = "../../CommonMedia/Maps/";
	
	


};

enum class RBActionState { NONE, PANNING, PAINTING, BRUSHING, MOVING, EDITING, TAGGING, PAINTINGWITHMASTER, DELETING };

enum class MasterPaintingDirection {RIGHT, LEFT, DOWN, UP, NONE};

enum class TileMasterSlaveType {MASTER, SLAVE, NONE};


#endif /* ANIMATION_HANDLER_HPP */