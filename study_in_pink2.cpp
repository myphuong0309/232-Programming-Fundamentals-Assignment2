#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

//3.1 MapElement
MapElement::MapElement(ElementType in_type) : type(in_type) {}

MapElement::~MapElement() {}

ElementType MapElement::getType() const {return type;}

Path::Path() : MapElement(ElementType::PATH) {}

Wall::Wall() : MapElement(ElementType::WALL) {}

FakeWall::FakeWall(int in_req_exp) : MapElement(ElementType::FAKE_WALL), req_exp(in_req_exp) {}

int FakeWall::getReqExp() const {return req_exp;}

//3.2 Map
Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls) : num_rows(num_rows), num_cols(num_cols) {
    if(num_rows == 0 || num_cols == 0) return;
    map = new MapElement**[num_rows];
    for(int i = 0; i < num_rows; i++) {
        map[i] = new MapElement*[num_cols];
        for(int j = 0; j < num_cols; j++) {
            map[i][j] = new Path();
        }
    }
    for(int i = 0; i < num_fake_walls; i++){
        int r = array_fake_walls[i].getRow(), c = array_fake_walls[i].getCol();
        if (r < num_rows && c < num_cols) {
            delete map[r][c];
            map[r][c] = new FakeWall((r * 257 + c * 139 + 89) % 900 + 1);
        }
        else continue;
    }
    for(int i = 0; i < num_walls; i++){
        int r = array_walls[i].getRow(), c = array_walls[i].getCol();
        if (r < num_rows && c < num_cols){
            delete map[r][c];
            map[r][c] = new Wall();
        }  
        else continue;
    }
}

Map::~Map() {
    if(num_rows == 0 || num_cols == 0) return;
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

bool Map::isValid(const Position & pos , MovingObject * mv_obj) const {
    int r = pos.getRow(), c = pos.getCol();
    if (r < 0 || r >= num_rows || c < 0 || c >= num_cols) return false;
    if(map[r][c]->getType() == ElementType::WALL) return false;
    else if(map[r][c]->getType() == ElementType::FAKE_WALL) {
        if(mv_obj->getName() == "Watson") {
            return dynamic_cast<Watson*>(mv_obj)->getEXP() > dynamic_cast<FakeWall*>(map[r][c])->getReqExp();
        }
        return map[r][c]->getType() != ElementType::WALL;
    }
    return true;
}

//3.3 Position
const Position Position::npos = Position(-1, -1);

Position::Position(int r, int c) : r(r), c(c) {}

Position::Position(const string &str_pos) {
    r = stoi(str_pos.substr(1, str_pos.find(',') - 1));
    c = stoi(str_pos.substr(str_pos.find(',') + 1, str_pos.length() - 2));
}

int Position::getRow() const {return r;}

int Position::getCol() const {return c;}

void Position::setRow(int r) {this->r = r;}

void Position::setCol(int c) {this->c = c;}

string Position::str() const {return "(" + to_string(r) + "," + to_string(c) + ")";}

bool Position::isEqual(int in_r, int in_c) const {return r == in_r && c == in_c;}

bool Position::isEqual(const Position& pos) const {return r == pos.getRow() && c == pos.getCol();}

//3.4 MovingObject
MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name) : index(index), pos(pos), map(map), name(name) {}

MovingObject::~MovingObject() {}

Position MovingObject::getCurrentPosition() const {return pos;}

string MovingObject::getName() const {return name;}

Character::Character(int index, const Position pos, Map * map, const string & name) : MovingObject(index, pos, map, name) {}

//3.5 Sherlock
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule), index_moving_rule(0) {
    bag = new SherlockBag(this);
    this->hp = init_hp;
    this->exp = init_exp;
    setHP(hp);
    setEXP(exp);
};

Sherlock::~Sherlock() {delete bag;}

Position Sherlock::getNextPosition() {
    char move = moving_rule[index_moving_rule % moving_rule.length()];
    Position next_pos = pos;
    if (move == 'U') next_pos.setRow(next_pos.getRow() - 1);
    if (move == 'D') next_pos.setRow(next_pos.getRow() + 1);
    if (move == 'L') next_pos.setCol(next_pos.getCol() - 1);
    if (move == 'R') next_pos.setCol(next_pos.getCol() + 1);
    if(map->isValid(next_pos, this)) return next_pos;
    else return Position::npos;
}

void Sherlock::move() {
    if(this->exp <= 0) return;
    if(this->getNextPosition().isEqual(Position::npos)) {
        index_moving_rule++;
        return;
    }
    pos = getNextPosition();
    index_moving_rule++;
}

string Sherlock::str() const {return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";}

int Sherlock::getHP() const {return hp;}

int Sherlock::getEXP() const {return exp;}

void Sherlock::setHP(int in_hp) {
    if (in_hp > 500) this->hp = 500;
    else if (in_hp < 0) this->hp = 0;
    else this->hp = in_hp;
}

void Sherlock::setEXP(int in_exp) {
    if (in_exp > 900) this->exp = 900;
    else if (in_exp < 0) this->exp = 0;
    else this->exp = in_exp;
}

BaseBag * Sherlock::getBag() const {return bag;}

void Sherlock::setPosition(Position new_pos) {pos = new_pos;}

//3.6 Watson
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule), hp(init_hp), exp(init_exp), index_moving_rule(0) {
    bag = new WatsonBag(this);
    this->hp = init_hp;
    this->exp = init_exp;
    setHP(hp);
    setEXP(exp);
};

Watson::~Watson() {delete bag;}

Position Watson::getNextPosition() {
    char move = moving_rule[index_moving_rule % moving_rule.length()];
    Position next_pos = pos;
    if (move == 'U') next_pos.setRow(next_pos.getRow() - 1);
    if (move == 'D') next_pos.setRow(next_pos.getRow() + 1);
    if (move == 'L') next_pos.setCol(next_pos.getCol() - 1);
    if (move == 'R') next_pos.setCol(next_pos.getCol() + 1);
    if (!map->isValid(next_pos, this)) return Position::npos;
    else return next_pos;
}

void Watson::move() {
    if(this->exp <= 0) return;
    if(this->getNextPosition().isEqual(Position::npos)) {
        index_moving_rule++;
        return;
    }
    pos = getNextPosition();
    index_moving_rule++;
}

string Watson::str() const {return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";}

int Watson::getHP() const {return hp;}

int Watson::getEXP() const {return exp;}

void Watson::setHP(int in_hp) {
    if (in_hp > 500) this->hp = 500;
    else if (in_hp < 0) this->hp = 0;
    else this->hp = in_hp;
}

void Watson::setEXP(int in_exp) {
    if (in_exp > 900) this->exp = 900;
    else if (in_exp < 0) this->exp = 0;
    else this->exp = in_exp;
}

BaseBag * Watson::getBag() const {return bag;}

//3.7 Criminal
Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, "Criminal"), sherlock(sherlock), watson(watson), past_pos(Position::npos), step(0) {};

Criminal::~Criminal() {}

Position Criminal::getNextPosition() {
    Position up = Position(pos.getRow() - 1, pos.getCol());
    Position down = Position(pos.getRow() + 1, pos.getCol());
    Position left = Position(pos.getRow(), pos.getCol() - 1);
    Position right = Position(pos.getRow(), pos.getCol() + 1);
    int up_dis = distance(up, sherlock->getCurrentPosition()) + distance(up, watson->getCurrentPosition());
    int down_dis = distance(down, sherlock->getCurrentPosition()) + distance(down, watson->getCurrentPosition());
    int left_dis = distance(left, sherlock->getCurrentPosition()) + distance(left, watson->getCurrentPosition());
    int right_dis = distance(right, sherlock->getCurrentPosition()) + distance(right, watson->getCurrentPosition());
    int max_dis = INT_MIN;
    if(map->isValid(up, this) && max_dis < up_dis) max_dis = up_dis;
    if(map->isValid(left, this) && max_dis < left_dis) max_dis = left_dis;
    if(map->isValid(down, this) && max_dis < down_dis) max_dis = down_dis;
    if(map->isValid(right, this) && max_dis < right_dis) max_dis = right_dis;
    if(max_dis == up_dis && map->isValid(up, this)) return up;
    if(max_dis == left_dis && map->isValid(left, this)) return left;
    if(max_dis == down_dis && map->isValid(down, this)) return down;
    if(max_dis == right_dis && map->isValid(right, this)) return right;
    return Position::npos;
}

void Criminal::move() {
    Position next_pos = getNextPosition();
    if (!next_pos.isEqual(Position::npos)) {
        past_pos = pos;
        pos = next_pos;
        step++;
    }
    else return;
}

string Criminal::str() const {return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";}

int Criminal::distance(const Position &pos1, const Position &pos2) const {
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
}

Position Criminal::getPastPosition() const {return past_pos;}

int Criminal::getStep() const {return step;}

//3.8 ArrayMovingObject
ArrayMovingObject::ArrayMovingObject(int capacity) : count(0), capacity(capacity) {
    arr_mv_objs = new MovingObject*[capacity];
    for(int i = 0; i < capacity; i++) arr_mv_objs[i] = nullptr;
}

ArrayMovingObject::~ArrayMovingObject(){
    for(int i = 3; i < count; i++) delete arr_mv_objs[i];
    delete[] arr_mv_objs;
}

bool ArrayMovingObject::isFull() const {return count == capacity;}

bool ArrayMovingObject::add(MovingObject *mv_obj) {
    if(!isFull()){
        arr_mv_objs[count] = mv_obj;
        if(count < capacity) count++;
        return true;
    }
    return false;
}

MovingObject *ArrayMovingObject::get(int index) const {return arr_mv_objs[index];}

int ArrayMovingObject::size() const {return count;}

string ArrayMovingObject::str() const {
    string str;
    str = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";";
    for(int i = 0; i < count; i++){
        str += get(i)->str();
        if(i != count - 1) str += ";";
    }
    str += "]";
    return str;
}

//3.9 Configuration
Configuration::Configuration(const string &filepath) : map_num_rows(0), map_num_cols(0), max_num_moving_objects(0) , num_walls(0), num_fake_walls(0), arr_walls(nullptr), arr_fake_walls(nullptr), sherlock_init_hp(0), sherlock_init_exp(0), watson_init_hp(0), watson_init_exp(0), sherlock_moving_rule(""), watson_moving_rule(""), sherlock_init_pos(Position::npos), watson_init_pos(Position::npos), criminal_init_pos(Position::npos){
    ifstream fileIn(filepath);
    string line;
    while(fileIn >> line){
        if(line.empty()) continue;
        string left = line.substr(0, line.find("=")), right = line.substr(line.find("=") + 1);
        if (left == "MAP_NUM_ROWS") map_num_rows = stoi(right);
        if (left == "MAP_NUM_COLS") map_num_cols = stoi(right);
        if (left == "MAX_NUM_MOVING_OBJECTS") max_num_moving_objects = stoi(right);
        if (left == "ARRAY_WALLS") {
            if (right.length() - 2 == 0) {
                num_walls = 0;
                arr_walls = nullptr;
            }
            else {
                num_walls = 1;
                for(int i = 0; i < right.length(); i++){
                    if(right[i] == ';') num_walls++;
                }
                arr_walls = new Position[num_walls];
                istringstream ss(right.substr(1, right.length() - 2));
                string temp; int i = 0;
                while (getline(ss, temp, ';')) {
                    arr_walls[i] = Position(temp);
                    i++;
                }
            }
        }
        if (left == "ARRAY_FAKE_WALLS") {
            if (right.length() - 2 == 0) {
                num_fake_walls = 0;
                arr_fake_walls = nullptr;
            }
            else {
                num_fake_walls = 1;
                for(int i = 0; i < right.length(); i++){
                    if(right[i] == ';') num_fake_walls++;
                }
                arr_fake_walls = new Position[num_fake_walls];
                istringstream ss(right.substr(1, right.length() - 2));
                string temp; int i = 0;
                while (getline(ss, temp, ';')) {
                    arr_fake_walls[i] = Position(temp);
                    i++;
                }
            }
        }
        if (left == "SHERLOCK_MOVING_RULE") sherlock_moving_rule = right;
        if (left == "SHERLOCK_INIT_POS") sherlock_init_pos = Position(right);
        if (left == "SHERLOCK_INIT_HP") sherlock_init_hp = stoi(right);
        if (left == "SHERLOCK_INIT_EXP") sherlock_init_exp = stoi(right);
        if (left == "WATSON_MOVING_RULE") watson_moving_rule = right;
        if (left == "WATSON_INIT_POS") watson_init_pos = Position(right);
        if (left == "WATSON_INIT_HP") watson_init_hp = stoi(right);
        if (left == "WATSON_INIT_EXP") watson_init_exp = stoi(right);
        if (left == "CRIMINAL_INIT_POS") criminal_init_pos = Position(right);
        if (left == "NUM_STEPS") num_steps = stoi(right);
        
    }
    fileIn.close();
}

Configuration::~Configuration() {
    if (num_walls != 0) delete[] arr_walls;
    if (num_fake_walls != 0) delete[] arr_fake_walls;
}

string Configuration::str() const {
    string str = "Configuration[\nMAP_NUM_ROWS=" + to_string(map_num_rows) 
    + "\nMAP_NUM_COLS=" + to_string(map_num_cols) 
    + "\nMAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects)
    + "\nNUM_WALLS=" + to_string(num_walls);
    if(num_walls != 0) {
        str += "\nARRAY_WALLS=[";
        for(int i = 0; i < num_walls; i++){
            str += arr_walls[i].str();
            if(i != num_walls - 1) str += ";";
        }
        str += "]";
    }
    else str += "\nARRAY_WALLS=[]";
    str = str + "\nNUM_FAKE_WALLS=" + to_string(num_fake_walls);
    if(num_fake_walls != 0) {
        str += "\nARRAY_FAKE_WALLS=[";
        for(int i = 0; i < num_fake_walls; i++){
            str += arr_fake_walls[i].str();
            if(i != num_fake_walls - 1) str += ";";
        }
        str += "]";
    }
    else str += "\nARRAY_FAKE_WALLS=[]";
    str = str + "\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule
    + "\nSHERLOCK_INIT_POS=" + sherlock_init_pos.str() 
    + "\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) 
    + "\nSHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) 
    + "\nWATSON_MOVING_RULE=" + watson_moving_rule 
    + "\nWATSON_INIT_POS=" + watson_init_pos.str() 
    + "\nWATSON_INIT_HP=" + to_string(watson_init_hp) 
    + "\nWATSON_INIT_EXP=" + to_string(watson_init_exp) 
    + "\nCRIMINAL_INIT_POS=" + criminal_init_pos.str() 
    + "\nNUM_STEPS=" + to_string(num_steps) + "\n]"; 
    return str;
}

//3.10 Robot
Robot::Robot(int index, const Position & init_pos, Map * map, Criminal * criminal, const string & name) : MovingObject(index, init_pos, map, "Robot"), criminal(criminal) {
    this->init_pos = init_pos;
    int p = init_pos.getRow() * init_pos.getCol();
    int s = getSumDigit(p);
    if(s == 0 || s == 1) item = new MagicBook();
    else if(s == 2 || s == 3) item = new EnergyDrink();
    else if(s == 4 || s == 5) item = new FirstAid();
    else if(s == 6 || s == 7) item = new ExcemptionCard();
    else {
        int t = (init_pos.getRow() * 11 + init_pos.getCol()) % 4;
        if(t == 0) item = new PassingCard("RobotS");
        else if(t == 1) item = new PassingCard("RobotC");
        else if(t == 2) item = new PassingCard("RobotSW");
        else item = new PassingCard("all");
    }
};

Robot::~Robot() {delete item;}

int Robot::getSumDigit(int num) const {
    int sum = 0;
    while(num != 0){
        sum += num%10;
        num/=10;
    }
    if (sum <= 9) return sum;
    return getSumDigit(sum);
}

void Robot::move() {
    Position next_pos = getNextPosition();
    if (!next_pos.isEqual(Position::npos)) pos = next_pos;
    else return;
}

RobotType Robot::getType() const {return robot_type;}

BaseItem * Robot::getItem() const {return item;}

RobotC::RobotC(int index, const Position & init_pos, Map * map, Criminal * criminal) : Robot(index, init_pos, map, criminal) {
    robot_type = RobotType::C;
}

Position RobotC::getNextPosition() {return criminal->getPastPosition();}

string RobotC::str() const {return "Robot[pos=" + pos.str() + ";type=C;dist=]";}

int RobotC::getDistance(Sherlock *sherlock) {return criminal->distance(pos, sherlock->getCurrentPosition());}

int RobotC::getDistance(Watson *watson) {return criminal->distance(pos, watson->getCurrentPosition());}

int RobotC::getDistance() const {return 0;}

RobotS::RobotS ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock ) : Robot(index, init_pos, map, criminal), sherlock(sherlock) {
    robot_type = RobotType::S;
}

Position RobotS::getNextPosition() {
    Position up = Position (pos.getRow() - 1, pos.getCol());
    Position down = Position (pos.getRow() + 1, pos.getCol());
    Position left = Position (pos.getRow(), pos.getCol() - 1);
    Position right = Position (pos.getRow(), pos.getCol() + 1);
    int up_dis = criminal->distance(up, sherlock->getCurrentPosition());
    int down_dis = criminal->distance(down, sherlock->getCurrentPosition());
    int left_dis = criminal->distance(left, sherlock->getCurrentPosition());
    int right_dis = criminal->distance(right, sherlock->getCurrentPosition());
    int min_dis = INT_MAX;
    if(map->isValid(up, this) && min_dis > up_dis) min_dis = up_dis;
    if(map->isValid(right, this) && min_dis > right_dis) min_dis = right_dis;
    if(map->isValid(down, this) && min_dis > down_dis) min_dis = down_dis;
    if(map->isValid(left, this) && min_dis > left_dis) min_dis = left_dis;
    if(min_dis == up_dis && map->isValid(up, this)) return up;
    if(min_dis == right_dis && map->isValid(right, this)) return right;
    if(min_dis == down_dis && map->isValid(down, this)) return down;
    if(min_dis == left_dis && map->isValid(left, this)) return left;
    return Position::npos;
}

string RobotS::str() const {return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(getDistance()) + "]";}

int RobotS::getDistance() const {return criminal->distance(pos, sherlock->getCurrentPosition());}

RobotW::RobotW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Watson * watson ) : Robot(index, init_pos, map, criminal), watson(watson) {
    robot_type = RobotType::W;
}

Position RobotW::getNextPosition() {
    Position up = Position(pos.getRow() - 1, pos.getCol());
    Position down = Position(pos.getRow() + 1, pos.getCol());
    Position left = Position(pos.getRow(), pos.getCol() - 1);
    Position right = Position(pos.getRow(), pos.getCol() + 1);
    int up_dis = criminal->distance(up, watson->getCurrentPosition());
    int down_dis = criminal->distance(down, watson->getCurrentPosition());
    int left_dis = criminal->distance(left, watson->getCurrentPosition());
    int right_dis = criminal->distance(right, watson->getCurrentPosition());
    int min_dis = INT_MAX;
    if(map->isValid(up, this) && min_dis > up_dis) min_dis = up_dis;
    if(map->isValid(right, this) && min_dis > right_dis) min_dis = right_dis;
    if(map->isValid(down, this) && min_dis > down_dis) min_dis = down_dis;
    if(map->isValid(left, this) && min_dis > left_dis) min_dis = left_dis;
    if(min_dis == up_dis && map->isValid(up, this)) return up;
    if(min_dis == right_dis && map->isValid(right, this)) return right;
    if(min_dis == down_dis && map->isValid(down, this)) return down;
    if(min_dis == left_dis && map->isValid(left, this)) return left;
    return Position::npos;
}

string RobotW::str() const {return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";}

int RobotW::getDistance() const {return criminal->distance(pos, watson->getCurrentPosition());}

RobotSW::RobotSW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson * watson ) : Robot(index, init_pos, map, criminal), sherlock(sherlock), watson(watson) {
    robot_type = RobotType::SW;
}

Position RobotSW::getNextPosition() {
    Position up = Position(pos.getRow() - 2, pos.getCol());
    Position up_right = Position(pos.getRow() - 1, pos.getCol() + 1);
    Position right = Position(pos.getRow(), pos.getCol() + 2);
    Position right_down = Position(pos.getRow() + 1, pos.getCol() + 1);
    Position down = Position(pos.getRow() + 2, pos.getCol());
    Position down_left = Position(pos.getRow() + 1, pos.getCol() - 1);
    Position left = Position(pos.getRow(), pos.getCol() - 2);
    Position left_up = Position(pos.getRow() - 1, pos.getCol() - 1);
    int up_dis = criminal->distance(up, sherlock->getCurrentPosition()) + criminal->distance(up, watson->getCurrentPosition());
    int up_right_dis = criminal->distance(up_right, sherlock->getCurrentPosition()) + criminal->distance(up_right, watson->getCurrentPosition());
    int right_dis = criminal->distance(right, sherlock->getCurrentPosition()) + criminal->distance(right, watson->getCurrentPosition());
    int right_down_dis = criminal->distance(right_down, sherlock->getCurrentPosition()) + criminal->distance(right_down, watson->getCurrentPosition());
    int down_dis = criminal->distance(down, sherlock->getCurrentPosition()) + criminal->distance(down, watson->getCurrentPosition());
    int down_left_dis = criminal->distance(down_left, sherlock->getCurrentPosition()) + criminal->distance(down_left, watson->getCurrentPosition());
    int left_dis = criminal->distance(left, sherlock->getCurrentPosition()) + criminal->distance(left, watson->getCurrentPosition());
    int left_up_dis = criminal->distance(left_up, sherlock->getCurrentPosition()) + criminal->distance(left_up, watson->getCurrentPosition());
    int min_dis = INT_MAX;
    if(map->isValid(up, this) && min_dis > up_dis) min_dis = up_dis;
    if(map->isValid(up_right, this) && min_dis > up_right_dis) min_dis = up_right_dis;
    if(map->isValid(right, this) && min_dis > right_dis) min_dis = right_dis;
    if(map->isValid(right_down, this) && min_dis > right_down_dis) min_dis = right_down_dis;
    if(map->isValid(down, this) && min_dis > down_dis) min_dis = down_dis;
    if(map->isValid(down_left, this) && min_dis > down_left_dis) min_dis = down_left_dis;
    if(map->isValid(left, this) && min_dis > left_dis) min_dis = left_dis;
    if(map->isValid(left_up, this) && min_dis > left_up_dis) min_dis = left_up_dis;
    
    if(min_dis == up_dis && map->isValid(up, this)) return up;
    if(min_dis == up_right_dis && map->isValid(up_right, this)) return up_right;
    if(min_dis == right_dis && map->isValid(right, this)) return right;
    if(min_dis == right_down_dis && map->isValid(right_down, this)) return right_down;
    if(min_dis == down_dis && map->isValid(down, this)) return down;
    if(min_dis == down_left_dis && map->isValid(down_left, this)) return down_left;
    if(min_dis == left_dis && map->isValid(left, this)) return left;
    if(min_dis == left_up_dis && map->isValid(left_up, this)) return left_up;
    return Position::npos;
}

string RobotSW::str() const {return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";}

int RobotSW::getDistance() const {return criminal->distance(pos, sherlock->getCurrentPosition()) + criminal->distance(pos, watson->getCurrentPosition());}

//3.11 BaseItem
BaseItem::BaseItem(ItemType in_item_type) : item_type(in_item_type) {}

ItemType BaseItem::getType() const {return item_type;}

MagicBook::MagicBook() : BaseItem(ItemType::MAGIC_BOOK) {}

string MagicBook::str() const {return "MagicBook";}

bool MagicBook::canUse ( Character * obj , Robot * robot ){
    if(obj->getName() == "Sherlock" && robot == nullptr) {
        if(dynamic_cast<Sherlock*>(obj)->getEXP() <= 350) return true;
    }
    else if(obj->getName() == "Watson" && robot == nullptr) {
        if(dynamic_cast<Watson*>(obj)->getEXP() <= 350) return true;
    }
    return false;
}

void MagicBook::use ( Character * obj , Robot * robot ) {
    if(this->canUse(obj, robot)) {
        if(obj->getName() == "Sherlock") dynamic_cast<Sherlock*>(obj)->setEXP(ceil((float) dynamic_cast<Sherlock*>(obj)->getEXP() * 1.25));
        else if(obj->getName() == "Watson") dynamic_cast<Watson*>(obj)->setEXP(ceil((float) dynamic_cast<Watson*>(obj)->getEXP() * 1.25));
    }
}

EnergyDrink::EnergyDrink() : BaseItem(ItemType::ENERGY_DRINK) {}

string EnergyDrink::str() const {return "EnergyDrink";}

bool EnergyDrink::canUse ( Character * obj , Robot * robot ){
    if(obj->getName() == "Sherlock" && robot == nullptr) {
        if(dynamic_cast<Sherlock*>(obj)->getHP() <= 100) return true;
    }
    else if(obj->getName() == "Watson" && robot == nullptr) {
        if(dynamic_cast<Watson*>(obj)->getHP() <= 100) return true;
    }
    return false;
}

void EnergyDrink::use ( Character * obj , Robot * robot ) {
    if(this->canUse(obj, robot)) {
        if(obj->getName() == "Sherlock") dynamic_cast<Sherlock*>(obj)->setHP(ceil((float)dynamic_cast<Sherlock*>(obj)->getHP() * 1.2));
        else if(obj->getName() == "Watson") dynamic_cast<Watson*>(obj)->setHP(ceil((float)dynamic_cast<Watson*>(obj)->getHP() * 1.2));
    }
}

FirstAid::FirstAid() : BaseItem(ItemType::FIRST_AID) {}

string FirstAid::str() const {return "FirstAid";}

bool FirstAid::canUse ( Character * obj , Robot * robot ){
    if(obj->getName() == "Sherlock" && robot == nullptr) {
        if(dynamic_cast<Sherlock*>(obj)->getEXP() <= 350 || dynamic_cast<Sherlock*>(obj)->getHP() <= 100) return true;
    }
    else if(obj->getName() == "Watson" && robot == nullptr) {
        if(dynamic_cast<Watson*>(obj)->getEXP() <= 350 || dynamic_cast<Watson*>(obj)->getHP() <= 100) return true;
    }
    return false;
}

void FirstAid::use ( Character * obj , Robot * robot ) {
    if(this->canUse(obj, robot)) {
        if(obj->getName() == "Sherlock")
            dynamic_cast<Sherlock*>(obj)->setHP(ceil((float)dynamic_cast<Sherlock*>(obj)->getHP() * 1.5));
        else if(obj->getName() == "Watson") 
            dynamic_cast<Watson*>(obj)->setHP(ceil((float)dynamic_cast<Watson*>(obj)->getHP() * 1.5));
    }
}

ExcemptionCard::ExcemptionCard() : BaseItem(ItemType::EXCEMPTION_CARD) {}

string ExcemptionCard::str() const {return "ExcemptionCard";}

bool ExcemptionCard::canUse ( Character * obj , Robot * robot ){
    if(obj->getName() == "Sherlock") 
        return dynamic_cast<Sherlock*>(obj)->getHP() % 2 != 0 && robot != nullptr;
    return false;
}

void ExcemptionCard::use ( Character * obj , Robot * robot ) {}

PassingCard::PassingCard(string in_challenge) : BaseItem(ItemType::PASSING_CARD), challenge(in_challenge){}

string PassingCard::str() const {return "PassingCard";}

bool PassingCard::canUse ( Character * obj , Robot * robot ){
    if(obj->getName() == "Watson") 
        return dynamic_cast<Watson*>(obj)->getHP() % 2 == 0 && robot != nullptr;
    return false;
}

void PassingCard::use ( Character * obj , Robot * robot ) {
    if(canUse(obj, robot)) {
        if (challenge == "all" || challenge == getRobotType(robot)) return;
        else dynamic_cast<Watson*>(obj)->setEXP(dynamic_cast<Watson*>(obj)->getEXP() - 50);
    }
}

string PassingCard::getRobotType(Robot* robot) const {
    if(robot->getType() == C) return "RobotC";
    else if(robot->getType() == S) return "RobotS";
    else if(robot->getType() == W) return "RobotW";
    else return "RobotSW";
}

//3.12 BaseBag
BaseBag::BaseBag(int in_capacity) : size(0), capacity(in_capacity), head(nullptr) {}

BaseBag::~BaseBag() {
    Node* temp = head;
    for(int i = 0; i < size; i++) { 
        Node* next_temp = temp->next;
        delete temp;
        temp = next_temp;
    }
}

bool BaseBag::insert(BaseItem * item) {
    Node* newNode = new Node(item);
    if (size == 0) head = newNode;
    else if (size == capacity) return false;
    else {
        newNode->next = head;
        head = newNode;
    } 
    size++;
    return true;
}

bool BaseBag::checkItem(ItemType in_item_type) {
    Node* temp = head;
    for(int i = 0; i < size; i++){
        if(temp->item->getType() == in_item_type) return true;
        temp = temp->next;
    }
    return false;
}

BaseItem * BaseBag::get(ItemType in_item_type) {
    Node *curr = head, *prev = nullptr;
    for(int i = 0; i < size; i++){
        if(curr->item->getType() == in_item_type){
            BaseItem* item = curr->item;
            if(prev != nullptr) {
                Node* temp = head;
                prev->next = temp;
                head = head->next;
                temp->next = curr->next;
                curr->next = head;
            }
            else head = curr->next;
            delete curr;
            size--;
            return item;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

string BaseBag::str() const {
    string str = "Bag[count=" + to_string(size) + ";";
    Node* cur = head;
    for(int i = 0; i < size; i++){
        str += cur->item->str();
        if(i != size - 1) str += ",";
        cur = cur->next;
    }
    str += "]";
    return str;
}

BaseItem * BaseBag::get() {
    Node* curr = head, *prev = nullptr;
    for(int i = 0; i < size; i++){
        if(curr->item->canUse(obj, nullptr)){
            BaseItem* item = curr->item;
            if(prev != nullptr) {
                Node* temp = head;
                prev->next = temp;
                head = head->next;
                temp->next = curr->next;
                curr->next = head;
            }
            else head = curr->next;
            delete curr;
            size--;
            return item;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

SherlockBag::SherlockBag(Sherlock * sherlock) : BaseBag(13), sherlock(sherlock) {
    this->obj = sherlock;
}

WatsonBag::WatsonBag(Watson * watson) : BaseBag(15), watson(watson) {
    this->obj = watson;
}

//3.13 StudyPinkProgram
StudyPinkProgram::StudyPinkProgram(const string & config_file_path){
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
}

StudyPinkProgram::~StudyPinkProgram(){
    delete map;
    delete config;
    delete sherlock;
    delete watson;
    delete criminal;
    delete arr_mv_objs;
}

bool StudyPinkProgram::isStop() const {
    if(watson->getHP() == 1 || sherlock->getHP() == 1) return true; 
    if(criminal->getCurrentPosition().isEqual(sherlock->getCurrentPosition()) || criminal->getCurrentPosition().isEqual(watson->getCurrentPosition())) return true;
    return false;
}

void StudyPinkProgram::run(bool verbose) {
    if(isStop()) {
        printResult();
        return;
    }
    for (int istep = 0; istep < config->num_steps; ++istep) {
        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            bool check;
            arr_mv_objs->get(i)->move();
            if (isStop()) {
                printStep(istep);
                printResult();
                return;
            }
            if (arr_mv_objs->get(i)->getName() == "Criminal") {
                if(criminal->getStep() == 3) {
                    arr_mv_objs->add(new RobotC(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal));
                }
                else if (criminal->getStep() % 3 == 0 && criminal->getStep() > 3) {
                    int sherlock_dis = criminal->distance(criminal->getPastPosition(), sherlock->getCurrentPosition());
                    int watson_dis = criminal->distance(criminal->getPastPosition(), watson->getCurrentPosition());
                    if (sherlock_dis < watson_dis) arr_mv_objs->add(new RobotS(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal, sherlock));
                    else if (sherlock_dis > watson_dis) arr_mv_objs->add(new RobotW(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal, watson));
                    else arr_mv_objs->add(new RobotSW(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal, sherlock, watson));
                }
            }
            check = meet(i);
            if (check) {
                printStep(istep);
                printResult();
                return;
            }
            if (verbose) {
                printStep(istep);
            }
        }
    }
    printResult();
}

bool StudyPinkProgram::meet(int index) {
    for(int i = 0; i < arr_mv_objs->size(); i++) {
        if(i == index) continue;
        if(arr_mv_objs->get(index)->getCurrentPosition().isEqual(arr_mv_objs->get(i)->getCurrentPosition())) {
            if(arr_mv_objs->get(index)->getName() == "Sherlock") {
                if(arr_mv_objs->get(i)->getName() == "Robot") {
                    BaseItem* item = nullptr;
                    bool use = false;
                    if(sherlock->getBag()->checkItem(EXCEMPTION_CARD) && sherlock->getHP() % 2 != 0){
                        sherlock->getBag()->get(EXCEMPTION_CARD);
                        use = true;
                    }
                    if(dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() != RobotType::W) {
                        if(dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() == RobotType::C) {
                            bool check;
                            if(sherlock->getEXP() > 500) {
                                sherlock->setPosition(criminal->getCurrentPosition());
                                check = true;
                            }
                            else {
                                sherlock->getBag()->insert(dynamic_cast<RobotC*>(arr_mv_objs->get(i))->getItem());
                                check = false;
                            }
                            if(check) {
                                item = sherlock->getBag()->get();
                                if(item != nullptr) item->use(sherlock, nullptr);
                                return check;
                            }
                        }
                        if(dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() == RobotType::S) {
                            if(sherlock->getEXP() > 400) sherlock->getBag()->insert(dynamic_cast<RobotS*>(arr_mv_objs->get(i))->getItem());
                            else if(use == false) sherlock->setEXP(ceil((float)sherlock->getEXP() * 0.9));
                        }
                        if(dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() == RobotType::SW) {
                            if(sherlock->getEXP() > 300 && sherlock->getHP() > 335) sherlock->getBag()->insert(dynamic_cast<RobotSW*>(arr_mv_objs->get(i))->getItem());
                            else if(use == false) {
                                sherlock->setHP(ceil((float)sherlock->getHP() * 0.85));
                                sherlock->setEXP(ceil((float)sherlock->getEXP() * 0.85));
                            }
                        }
                    }
                    else sherlock->getBag()->insert(dynamic_cast<RobotW*>(arr_mv_objs->get(i))->getItem());
                    item = sherlock->getBag()->get();
                    if(item != nullptr) item->use(sherlock, nullptr);
                }
                if(arr_mv_objs->get(i)->getName() == "Watson") {
                    if(sherlock->getBag()->checkItem(PASSING_CARD) && watson->getBag()->checkItem(EXCEMPTION_CARD)){
                        while(sherlock->getBag()->checkItem(PASSING_CARD)) watson->getBag()->insert(sherlock->getBag()->get(PASSING_CARD));
                        while(watson->getBag()->checkItem(EXCEMPTION_CARD)) sherlock->getBag()->insert(watson->getBag()->get(EXCEMPTION_CARD));
                    }
                }
                if(arr_mv_objs->get(i)->getName() == "Criminal") return true;
            } 
            if(arr_mv_objs->get(index)->getName() == "Watson") {
                if(arr_mv_objs->get(i)->getName() == "Robot") {
                    BaseItem * item = nullptr;
                    if(dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() == RobotType::C) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0)
                            watson->getBag()->get(PASSING_CARD)->use(watson, dynamic_cast<RobotC*>(arr_mv_objs->get(i)));
                        watson->getBag()->insert(dynamic_cast<RobotC*>(arr_mv_objs->get(i))->getItem());
                    }
                    if (dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() == RobotType::W) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0) {
                            watson->getBag()->get(PASSING_CARD)->use(watson, dynamic_cast<RobotW*>(arr_mv_objs->get(i)));
                            watson->getBag()->insert(dynamic_cast<RobotW*>(arr_mv_objs->get(i))->getItem());
                        }
                        else if(watson->getHP() > 350) watson->getBag()->insert(dynamic_cast<RobotW*>(arr_mv_objs->get(i))->getItem());
                        else watson->setHP(ceil((float) watson->getHP() * 0.95));
                    }
                    if (dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() == RobotType::SW) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0) {
                            watson->getBag()->get(PASSING_CARD)->use(watson, dynamic_cast<RobotSW*>(arr_mv_objs->get(i)));
                            watson->getBag()->insert(dynamic_cast<RobotSW*>(arr_mv_objs->get(i))->getItem());
                        }
                        else if(watson->getHP() > 165 && watson->getEXP() > 600) watson->getBag()->insert(dynamic_cast<RobotSW*>(arr_mv_objs->get(i))->getItem());
                        else {
                            watson->setHP(ceil((float) watson->getHP() * 0.85));
                            watson->setEXP(ceil((float) watson->getEXP() * 0.85));
                        }
                    }
                    if (dynamic_cast<Robot*>(arr_mv_objs->get(i))->getType() == RobotType::S) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0) {
                            watson->getBag()->get(PASSING_CARD)->use(watson, dynamic_cast<RobotS*>(arr_mv_objs->get(i)));
                        }
                    }
                    item = watson->getBag()->get();
                    if(item != nullptr) item->use(watson, nullptr);
                }
                if(arr_mv_objs->get(i)->getName() == "Sherlock") {
                    if(watson->getBag()->checkItem(EXCEMPTION_CARD) && sherlock->getBag()->checkItem(PASSING_CARD)){
                        while(sherlock->getBag()->checkItem(PASSING_CARD)) watson->getBag()->insert(sherlock->getBag()->get(PASSING_CARD));
                        while(watson->getBag()->checkItem(EXCEMPTION_CARD)) sherlock->getBag()->insert(watson->getBag()->get(EXCEMPTION_CARD));
                    }
                }
                if(arr_mv_objs->get(i)->getName() == "Criminal") return true;
            }
            if(arr_mv_objs->get(index)->getName() == "Robot") {
                if(arr_mv_objs->get(i)->getName() == "Sherlock") {
                    BaseItem* item = nullptr;
                    bool use = false;
                    if(sherlock->getBag()->checkItem(EXCEMPTION_CARD) && sherlock->getHP() % 2 != 0){
                        sherlock->getBag()->get(EXCEMPTION_CARD);
                        use = true;
                    }
                    if(dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() != RobotType::W) {
                        if(dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() == RobotType::C) {
                            bool check;
                            if(sherlock->getEXP() > 500) {
                                sherlock->setPosition(criminal->getCurrentPosition());
                                check = true;
                            }
                            else {
                                sherlock->getBag()->insert(dynamic_cast<RobotC*>(arr_mv_objs->get(index))->getItem());
                                check = false;
                            }
                            if(check) {
                                item = sherlock->getBag()->get();
                                if(item != nullptr) item->use(sherlock, nullptr);
                                return check;
                            }
                        }
                        if(dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() == RobotType::S) {
                            if(sherlock->getEXP() > 400) sherlock->getBag()->insert(dynamic_cast<RobotS*>(arr_mv_objs->get(index))->getItem());
                            else if(use == false) sherlock->setEXP(ceil((float)sherlock->getEXP() * 0.9));
                        }
                        if(dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() == RobotType::SW) {
                            if(sherlock->getEXP() > 300 && sherlock->getHP() > 335) sherlock->getBag()->insert(dynamic_cast<RobotSW*>(arr_mv_objs->get(index))->getItem());
                            else if(use == false) {
                                sherlock->setHP(ceil((float)sherlock->getHP() * 0.85));
                                sherlock->setEXP(ceil((float)sherlock->getEXP() * 0.85));
                            }
                        }
                    }
                    else sherlock->getBag()->insert(dynamic_cast<RobotW*>(arr_mv_objs->get(index))->getItem());
                    item = sherlock->getBag()->get();
                    if(item != nullptr) item->use(sherlock, nullptr);
                }
                if(arr_mv_objs->get(i)->getName() == "Watson") {
                    BaseItem * item = nullptr;
                    if(dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() == RobotType::C) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0){
                            item = watson->getBag()->get(PASSING_CARD);
                            item->use(watson, dynamic_cast<RobotC*>(arr_mv_objs->get(index)));
                        }
                        watson->getBag()->insert(dynamic_cast<RobotC*>(arr_mv_objs->get(index))->getItem());
                    }
                    if (dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() == RobotType::W) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0) {
                            watson->getBag()->get(PASSING_CARD)->use(watson, dynamic_cast<RobotW*>(arr_mv_objs->get(index)));
                            watson->getBag()->insert(dynamic_cast<RobotW*>(arr_mv_objs->get(index))->getItem());
                        }
                        else if(watson->getHP() > 350) watson->getBag()->insert(dynamic_cast<RobotW*>(arr_mv_objs->get(index))->getItem());
                        else watson->setHP(ceil((float)watson->getHP() * 0.95));
                    }
                    if (dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() == RobotType::SW) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0) {
                            watson->getBag()->get(PASSING_CARD)->use(watson, dynamic_cast<RobotSW*>(arr_mv_objs->get(index)));
                            watson->getBag()->insert(dynamic_cast<RobotSW*>(arr_mv_objs->get(index))->getItem());
                        }
                        else if(watson->getHP() > 165 && watson->getEXP() > 600) watson->getBag()->insert(dynamic_cast<RobotSW*>(arr_mv_objs->get(index))->getItem());
                        else {
                            watson->setHP(ceil((float)watson->getHP() * 0.85));
                            watson->setEXP(ceil((float)watson->getEXP() * 0.85));
                        }
                    }
                    if (dynamic_cast<Robot*>(arr_mv_objs->get(index))->getType() == RobotType::S) {
                        if(watson->getBag()->checkItem(PASSING_CARD) && watson->getHP() % 2 == 0) {
                            watson->getBag()->get(PASSING_CARD)->use(watson, dynamic_cast<RobotS*>(arr_mv_objs->get(index)));
                        }
                    }
                    item = watson->getBag()->get();
                    if(item != nullptr) item->use(watson, nullptr);
                }
            } 
        }
    }
    return false;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////