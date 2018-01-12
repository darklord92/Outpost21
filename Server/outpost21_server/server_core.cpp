#include "server_core.h"
#include "json/json.hpp"
#include "entitylibrary/entity_library.h"
#include <fstream>

///SERVER CORE
void serverCore::CreateObjectAndAssetIndex(std::string inputAssetIndex) {
    int get_last_index = object_index.size();

    //add latest index!
    object_index[inputAssetIndex] = get_last_index;
    asset_index[get_last_index] = inputAssetIndex;
}


std::string serverCore::getAssetOfIndex(int inputObjIndex) {
    return asset_index[inputObjIndex];
}
int serverCore::getIndexOfAsset(std::string inputAstIndex) {
    return object_index[inputAstIndex];
}


void serverCore::securityLevelAdd( unsigned int index, std::string inputName, int inputColor) {

    securityLevel* newSec = new securityLevel;
    newSec->name = inputName;
    newSec->color = inputColor;

    //add to sec vector
    mapSecurityLevels[index] = newSec;
}

void serverCore::securityLevelRemove( unsigned int index) {
    //remove security level from map.
    if(mapSecurityLevels[index] == nullptr) {
        delete mapSecurityLevels[index];

        ///TODO!!!
        //update all entities to remove that clearance

        //set all doors with this clearance to remove it.
    }
}


void serverCore::entity_add(entity* entityToAdd) {
    for(unsigned int i = 0; i < entity_map.size()+1; i++) {
        if(i == entity_map.size()
        || entity_map[i] == nullptr) {

            std::cout << "Assigning entity number: " << i << std::endl;

            ///add at the end of list, or if a blank space is found
            entity_map[i] = entityToAdd;
            entityToAdd->entity_number = i;
            break;
        }
    }
};


void serverCore::entity_set(entity* entityToAdd, int entityNumberToAssign) {
    if(entityNumberToAssign >= entity_map.size() || entity_map[entityNumberToAssign] == nullptr) {

        std::cout << "Assigning entity number: " << entityNumberToAssign << std::endl;

        ///add at the end of list, or if a blank space is found
        entity_map[entityNumberToAssign] = entityToAdd;
        entityToAdd->entity_number = entityNumberToAssign;
    }
};


void serverCore::entity_remove(int entityNumberToRemove) {
    if(entityNumberToRemove >= 0
    && entity_map[entityNumberToRemove] != nullptr) {
        //place entity in abyss so the clients are repeatedly FORCED to unload it!
        entity* get_ent = entity_map[entityNumberToRemove];
        get_ent->x = serverObj.entity_deletion_abyss;
        get_ent->y = serverObj.entity_deletion_abyss;
    }
};


void serverCore::set_update_flag( int entityNumberToUpdate, int clientNumber, bool updateFlag) {
    if(entityNumberToUpdate >= 0
    && entity_map[entityNumberToUpdate] != nullptr) {
        //set the entities updateflag for this client!
        entity* get_ent = entity_map[entityNumberToUpdate];
        get_ent->needs_update[ clientNumber] = updateFlag;
    }
}



void serverCore::securityMapLoad(nlohmann::json j) {
    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
        nlohmann::json jsonSecurity = (nlohmann::json)*it;

        if(jsonSecurity.is_object()) {
            //add security level to vector
            std::cout << "====Loading Security level: " << std::endl;

            unsigned int getIndex = jsonSecurity.at("index").get<int>();
            std::cout << " |---------index: " << getIndex << std::endl;

            std::string getName = jsonSecurity.at("name").get<std::string>();
            std::cout << " |----------name: " << getName << std::endl;

            unsigned int getColor = jsonSecurity.at("color").get<int>();
            std::cout << " |---------color: " << getColor << std::endl;

            securityLevelAdd(getIndex,getName,getColor);
        }
    }
}


entity* serverCore::entityAssembleMapLoad(nlohmann::json a) {
    //this takes all build in variables and assigns them, leaving the others to be picked up by
    //the entity's personal variables
    int getEntityNumber = -1;
    std::string get_objectindex = "";
    double getx = 0;
    double gety = 0;
    double getprvx = 0;
    double getprvy = 0;
    float getdir = 0;
    double getspd = 0;
    bool getindestruct = false;

    bool get_disp = false;
    int get_disp_d = 0;
    double get_disp_x = 0;
    double get_disp_y = 0;

    int getinsideof = -1;
    int getgrabbing_entity = -1;
    bool getconstructed = false;
    bool get_SS_ignorewalls = false;
    double get_SS_bouncyness = 0;
    double get_SS_decelerator = 1;

    int get_isclass = 0;
    int get_issize = 0;
    bool get_isliquid = false;
    int get_containsmax = 0;
    int get_containssize = 0;
    bool get_containsliquid = false;
    int get_containsclass = 0;

    std::cout << "====Loading entity====" << std::endl;
    for (nlohmann::json::iterator it = a.begin(); it != a.end(); ++it) {
        //itterate through entity's properties
        //allows us to set the hard internal values
        //and the soft item specific values
        std::string jsonKey = it.key();


        if(jsonKey == "entity_number") {
            getEntityNumber = a.at(jsonKey).get<int>();
            std::cout << " |------entityID: " << getEntityNumber << std::endl;
        }
        else if(jsonKey == "object_index") {
            get_objectindex = a.at(jsonKey).get<std::string>();
            std::cout << " |-----obj index: " << get_objectindex << std::endl;
        }
        else if(jsonKey == "x") {
            getx = a.at(jsonKey).get<double>();
            std::cout << " |-------------x: " << getx << std::endl;
        }
        else if(jsonKey == "y") {
            gety = a.at(jsonKey).get<double>();
            std::cout << " |-------------y: " << gety << std::endl;
        }
        else if(jsonKey == "last_update_x") {
            //ignore this,
        }
        else if(jsonKey == "last_update_y") {
            //ignore this,
        }
        else if(jsonKey == "needs_update") {
            //ignore this,
        }
        else if(jsonKey == "entity_last_process_cycle") {
            //ignore this,
        }
        else if(jsonKey == "dir") {
            getdir = a.at(jsonKey).get<float>();
            std::cout << " |-----------dir: " << getdir << std::endl;
        }
        else if(jsonKey == "spd") {
            getspd = a.at(jsonKey).get<double>();
            std::cout << " |-----------spd: " << getspd << std::endl;
        }
        else if(jsonKey == "indestructable") {
            getindestruct = (bool)a.at(jsonKey).get<int>();
            std::cout << " |----indestruct: " << getindestruct << std::endl;
        }
        else if(jsonKey == "inside_of_id") {
            getinsideof = a.at(jsonKey).get<int>();
            std::cout << " |-----insideent: " << getinsideof << std::endl;
        }
        else if(jsonKey == "grabbing_entity") {
            getgrabbing_entity = a.at(jsonKey).get<int>();
            std::cout << " |---grabbingent: " << getgrabbing_entity << std::endl;
        }
        else if(jsonKey == "constructed") {
            getconstructed = (bool)a.at(jsonKey).get<int>();
            std::cout << " |---constructed: " << getconstructed << std::endl;
        }
        else if(jsonKey == "SS_collision") {
            //ignore this,
        }
        else if(jsonKey == "SS_collision_ignores_walls") {
            get_SS_ignorewalls = (bool)a.at(jsonKey).get<int>();
            std::cout << " |---SS_ignwalls: " << get_SS_ignorewalls << std::endl;
        }
        else if(jsonKey == "SS_bouncyness") {
            get_SS_ignorewalls = a.at(jsonKey).get<double>();
            std::cout << " |---SS_bouncyns: " << get_SS_bouncyness << std::endl;
        }
        else if(jsonKey == "SS_decelerator") {
            get_SS_ignorewalls = a.at(jsonKey).get<double>();
            std::cout << " |---SS_decelera: " << get_SS_decelerator << std::endl;
        }
        else if(jsonKey == "security_clearance") {
            //ignore this,
        }
        else if(jsonKey == "contains_display") {
            get_disp = (bool)a.at(jsonKey).get<int>();
            std::cout << " |-------display: " << get_disp << std::endl;
        }
        else if(jsonKey == "contains_display_d") {
            get_disp_d = a.at(jsonKey).get<int>();
            std::cout << " |---display_dep: " << get_disp_d  << std::endl;
        }
        else if(jsonKey == "contains_display_x") {
            get_disp_x = a.at(jsonKey).get<double>();
            std::cout << " |-----display_x: " << get_disp_x << std::endl;
        }
        else if(jsonKey == "contains_display_y") {
            get_disp_y = a.at(jsonKey).get<double>();
            std::cout << " |-----display_y: " << get_disp_y << std::endl;
        }
        else if(jsonKey == "contains_max") {
            get_containsmax = a.at(jsonKey).get<int>();
            std::cout << " |---contain_max: " << get_containsmax << std::endl;
        }
        else if(jsonKey == "contains_size") {
            get_containssize = a.at(jsonKey).get<int>();
            std::cout << " |--contain_size: " << get_containssize << std::endl;
        }
        else if(jsonKey == "contains_type_liquid") {
            get_containsliquid = (bool)a.at(jsonKey).get<int>();
            std::cout << " |-contain_liqud: " << get_containsliquid << std::endl;
        }
        else if(jsonKey == "contains_class") {
            get_containsclass = a.at(jsonKey).get<int>();
            std::cout << " |-contain_class: " << get_containsclass << std::endl;
        }
        else if(jsonKey == "is_class") {
            get_isclass = a.at(jsonKey).get<int>();
            std::cout << " |-------isclass: " << get_isclass << std::endl;
        }
        else if(jsonKey == "is_size") {
            get_issize = a.at(jsonKey).get<int>();
            std::cout << " |--------issize: " << get_issize << std::endl;
        }
        else if(jsonKey == "is_liquid") {
            get_isliquid = (bool)a.at(jsonKey).get<int>();
            std::cout << " |-------isliqud: " << get_isliquid << std::endl;
        }
        else
        {
            std::cout << " |--Personal key: " << jsonKey << std::endl;
            ///TODO needs to tell between string and int

        }
        ///TODO add the rest of these entity assembly things. And then the generic case of personal vars!
    }

    //call entity create to make the base entity
    entity* make_entity = entityLibrary::entity_template_library(get_objectindex,getx,gety,getdir,getspd,getindestruct,getinsideof);
    make_entity->entity_set_inventorylimits(get_containsmax,get_containssize,get_issize,get_isliquid,get_containsliquid,get_isclass,get_containsclass);

    make_entity->entity_setConstructed( getconstructed);
    make_entity->entity_setGrabbed( getgrabbing_entity);

    make_entity->contains_display = get_disp;
    make_entity->contains_display_d = get_disp_d;
    make_entity->contains_display_x = get_disp_x;
    make_entity->contains_display_y = get_disp_y;

    make_entity->SS_collision_ignores_walls = get_SS_ignorewalls;
    make_entity->SS_bouncyness = get_SS_bouncyness;
    make_entity->SS_decelerator = get_SS_decelerator;

    //fill entity with personal data!


    //pass back entity
    return make_entity;
}


void serverCore::entityMapLoad(nlohmann::json e) {
    //extract entities from the json data!
    for (nlohmann::json::iterator it = e.begin(); it != e.end(); ++it) {
        nlohmann::json jsonEntity = (nlohmann::json)*it;

        if(jsonEntity.is_object()) {
            entity* make_entity = entityAssembleMapLoad( jsonEntity);

            serverObj.entity_set( make_entity, make_entity->entity_number); //add to main list
        }
    }
}



bool serverCore::gameMapLoad(std::string mapFilePath) {
    //file finished loading
    bool finishedLoading = false;

    // read a JSON file
    std::ifstream inputFile( mapFilePath);

    if(inputFile.good() == false) {
        std::cout << "Map file '" << mapFilePath <<"' not found." << std::endl;
    }
    else
    {
        nlohmann::json jsonData;
        inputFile >> jsonData;

        //extract security data first!
        securityMapLoad(jsonData["Security"]);

        //now for the entities!
        entityMapLoad(jsonData["Entities"]);

        //finally the map's walls!
    }

    return finishedLoading;
}




///ENTITY
entity::entity(std::string set_object_index,double set_x,double set_y, float set_dir, double set_spd, bool set_indestructable, int set_insideid) {
    //constructor
    std::string object_index = set_object_index;
    entity_number = -1; //index in entity vector
    x = set_x;
    y = set_y;
    last_update_x = set_x;
    last_update_y = set_y;
    dir = set_dir;
    spd = set_spd;
    inside_of_id = set_insideid;
    indestructable = set_indestructable;

    //set all update flags
    for (int i = 0; i < serverObj.server_maxplayers; i++)
    {
        needs_update[i] = true; //is a list of player flags!
    }
}


void entity::entity_set_inventorylimits( int inventory_size, int max_storeable_item_size, int item_size, bool is_a_liquid, bool contains_a_liquid, int item_class, int inventory_storage_class) {
    is_liquid = is_a_liquid;
    is_size = item_size; //item is a physical size of
    //self inventory
    contains_max = inventory_size; //inventory max size
    contains_type_liquid = contains_a_liquid;
    contains_size = max_storeable_item_size; //item physical size that can be held
}


void entity::entity_securityInit() {
    //set all security flags as disabled, entities calling security updates will correct this. This is just init.
    //largely because it is called before the inventory has been properly set for the object, so it can't check for
    //the security cards needed to set its clearances anyway.
    for(unsigned int i; i < serverObj.mapSecurityLevels.size(); i++) {
        securityLevel[ i] = false;
    }
}

void entity::entity_securityUpdate() {
    //blank out all security levels for safety
    for(unsigned int i; i < serverObj.mapSecurityLevels.size(); i++) {
        securityLevel[ i] = false;
    }

    ///TODO!!!

    //check inventory for cards in slots!

    //be sure to check if the security level exists, if not forcibly disable it!
}


void entity::entity_step() {
    //physics calculation, movement, and collisions
    if(entity_last_process_cycle != serverObj.entity_process_cycle) {


        //personal update
        entity_personal_step();

        //prevent update in the same frame somehow
        entity_last_process_cycle = serverObj.entity_process_cycle;
    }
}


void entity::entity_personal_step() {
    //enacts personal countdowns, or transforms it into other entities
    //for example, coffee cooling down and changing into cold coffee
}


void entity::entity_setConstructed(bool setCon){
    constructed = setCon;
}

bool entity::entity_getConstructed() {
    return constructed;
}

void entity::entity_setGrabbed(unsigned int entityNumber) {
    grabbing_entity = entityNumber;
}

unsigned int entity::entity_getGrabbed() {
    return grabbing_entity;
}