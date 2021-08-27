#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


/**
 * \brief Contains identifiers for objects that can be created in the editor.
 */
enum class WorldObjects
{
	WormSpawnPoint, //!< The point at which the worm may appear/spawn
	StaticPaperBlock, //!< A paper block that does not move, although it physically interacts with other physical objects
	DynamicPaperBlock, //!< A paper block that moves, falls, bounce and physically interacts with other physical objects
	Water, //!< Water that kills and destroys other objects
	DestructibleBlock, //!< Object that can be destroyed with a weapon
	
	Counter,
};

/**
 * \brief A helper class for handling objects inside the editor
 * and making them easier for the world to read.
 *
 * \warning Works are still in progress -- never used for now
 *
 * According to the idea so far, the class would be designed to
 * both help and relieve the "World" class from creating objects
 * based on identifiers.
 *
 * It would do the same for the "Editor" class, and would relieve
 * it of reading the file world and converting it into "NodeEditorObject"
 * objects, and writing them to the file.
 *
 * This would make all available objects available in this one class,
 * and there would be no need to jump around files to create additional objects.
 *
 * Unfortunately, this limits some possibilities, where, for example, "WormSpawnPoint"
 * is not exactly a "NodeScene" object, so returning objects as "NodeScene" should rather
 * not be done according to the rules of returning "NodeScene".
 *
 * This is problematic, which is why this class has not yet been implemented.
 */
//class InGameObjects
//{
//public:
//	void registerInGameObjects();
//	std::unique_ptr<NodeScene> createWorldObject(unsigned objectId);
//	NodeEditorObject&& createEditorObject(unsigned objectId);
//
//private:
//	std::map<unsigned, std::string> inGameObjects; //!< Map object identifiers to their names (text representations).
//	
//};
//
//inline void InGameObjects::registerInGameObjects()
//{
//	inGameObjects.insert({ static_cast<unsigned>(WorldObjects::WormSpawnPoint), "Worm Spawn Point" });
//	inGameObjects.insert({ static_cast<unsigned>(WorldObjects::StaticPaperBlock), "Static Paper Block" });
//	inGameObjects.insert({ static_cast<unsigned>(WorldObjects::DynamicPaperBlock), "Dynamic Paper Block" });
//}

#endif
