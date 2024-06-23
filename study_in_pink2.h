/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
    friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
    friend class TestStudyInPink;
public:
    Path();
};

class Wall : public MapElement {
    friend class TestStudyInPink;
public:
    Wall();
};

class FakeWall : public MapElement {
    friend class TestStudyInPink;
private:
    int req_exp;
public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map {
    friend class TestStudyInPink;
private:
    int num_rows, num_cols;
    MapElement *** map;
public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();
    bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
    friend class TestStudyInPink;
private:
    int r, c;
public:
    static const Position npos;

    Position(int r=0, int c=0);

    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
    bool isEqual(const Position & pos) const;
};

class MovingObject {
    friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;

public:
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    string getName() const;
};

class Character : public MovingObject{
    friend class TestStudyInPink;
public:
    Character(int index, const Position pos, Map * map, const string & name = "");
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
};

class Sherlock : public Character {
    friend class TestStudyInPink;
private:
    int hp, exp;
    string moving_rule;
    int index_moving_rule;
    BaseBag * bag;
public:
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    int getHP() const;
    int getEXP() const;
    void setHP(int in_hp);
    void setEXP(int in_exp);
    BaseBag * getBag() const;
    void setPosition(Position new_pos);
    ~Sherlock();
};

class Watson : public Character {
    friend class TestStudyInPink;
private:
    int hp, exp;
    string moving_rule;
    int index_moving_rule;
    BaseBag * bag;
public:
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    int getHP() const;
    int getEXP() const;
    void setHP(int in_hp);
    void setEXP(int in_exp);
    BaseBag * getBag() const;
    ~Watson();
};

class Criminal : public Character {
    friend class TestStudyInPink;
private:
    Sherlock * sherlock;
    Watson * watson;
    int step;
    Position past_pos;
public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    int distance(const Position & pos1, const Position & pos2) const;
    Position getPastPosition() const;
    int getStep() const;
    ~Criminal();
};

class ArrayMovingObject {
    friend class TestStudyInPink;
private:
    MovingObject ** arr_mv_objs;
    int count;
    int capacity;
public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
};

class Robot : public MovingObject {
    friend class TestStudyInPink;
protected:
    RobotType robot_type;
    BaseItem * item;
    Criminal * criminal;
    Position init_pos;
public:
    Robot(int index, const Position & init_pos, Map * map, Criminal * criminal, const string & name = "");
    ~Robot();
    int getSumDigit(int num) const;
    virtual Position getNextPosition() = 0;
    virtual void move();
    virtual string str() const = 0;
    virtual int getDistance() const = 0;
    virtual RobotType getType() const;
    BaseItem * getItem() const;
};

class RobotC : public Robot {
    friend class TestStudyInPink;
public:
    RobotC ( int index , const Position & init_pos , Map * map , Criminal * criminal ) ;
    Position getNextPosition() override;
    string str() const override;
    int getDistance() const override;
    int getDistance(Sherlock * sherlock);
    int getDistance(Watson * watson);
};

class RobotS : public Robot {
    friend class TestStudyInPink;
private:
    Sherlock * sherlock;
public:
    RobotS ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock ) ;
    Position getNextPosition() override;
    string str() const override;
    int getDistance() const override;
};

class RobotW : public Robot {
    friend class TestStudyInPink;
private:
    Watson * watson;
public:
    RobotW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Watson * watson ) ;
    Position getNextPosition() override;
    string str() const override;
    int getDistance() const override;
};

class RobotSW : public Robot {
    friend class TestStudyInPink;
private:
    Sherlock * sherlock;
    Watson * watson;
public:
    RobotSW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson * watson ) ;
    Position getNextPosition() override;
    string str() const override;
    int getDistance() const override;
};

class BaseItem {
    friend class TestStudyInPink;
protected:
    ItemType item_type;
public:
    BaseItem(ItemType in_item_type);
    virtual ItemType getType() const;
    virtual string str() const = 0;
    virtual bool canUse ( Character * obj , Robot * robot ) = 0;
    virtual void use ( Character * obj , Robot * robot ) = 0;
};

class MagicBook : public BaseItem {
    friend class TestStudyInPink;
public:
    MagicBook();
    string str() const override;
    bool canUse ( Character * obj , Robot * robot ) override;
    void use ( Character * obj , Robot * robot ) override;
};

class EnergyDrink : public BaseItem {
    friend class TestStudyInPink;
public:
    EnergyDrink();
    string str() const override;
    bool canUse ( Character * obj , Robot * robot ) override;
    void use ( Character * obj , Robot * robot ) override;
};

class FirstAid : public BaseItem {
    friend class TestStudyInPink;
public:
    FirstAid();
    string str() const override;
    bool canUse ( Character * obj , Robot * robot ) override;
    void use ( Character * obj , Robot * robot ) override;
};

class ExcemptionCard : public BaseItem {
    friend class TestStudyInPink;   
public:
    ExcemptionCard();
    string str() const override;
    bool canUse ( Character * obj , Robot * robot ) override;
    void use ( Character * obj , Robot * robot ) override;
};

class PassingCard : public BaseItem {
    friend class TestStudyInPink;
private: 
    string challenge;
public:
    PassingCard(string in_challenge);
    string str() const override;
    bool canUse ( Character * obj , Robot * robot ) override;
    void use ( Character * obj , Robot * robot ) override;
    string getRobotType(Robot* robot) const;
};

class Node {
    friend class TestStudyInPink;
public:
    BaseItem * item;
    Node * next;    
    Node(BaseItem * in_item, Node * next = nullptr) : item(in_item){};
};

class BaseBag {
    friend class TestStudyInPink;
protected:
    int size;
    int capacity;
    Node * head;
    Character * obj;
public:
    BaseBag(int in_capacity);
    virtual ~BaseBag();
    virtual bool insert(BaseItem * item);
    virtual BaseItem * get ();
    virtual BaseItem * get (ItemType in_item_type);
    virtual string str() const;
    bool checkItem(ItemType in_item_type); 
};

class SherlockBag : public BaseBag {
    friend class TestStudyInPink;
private:
    Sherlock * sherlock;
public:
    SherlockBag(Sherlock * sherlock);
};

class WatsonBag : public BaseBag {
    friend class TestStudyInPink;
private:
    Watson * watson;
public:
    WatsonBag(Watson * watson);
};

class Configuration {
    friend class StudyPinkProgram;
    friend class TestStudyInPink;
private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;
public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};

class StudyPinkProgram
{
    friend class TestStudyInPink;

private:
    // Sample attributes
    Configuration *config;
    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;
    Map *map;
    ArrayMovingObject *arr_mv_objs;
public:
    StudyPinkProgram(const string &config_file_path);
    bool isStop() const;
    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }
    void run(bool verbose);
    bool meet(int index);
    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
