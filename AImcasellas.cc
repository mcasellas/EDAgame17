#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME mcasellas


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }
    
  /**
   * Types and attributes for your player can be defined here.
   */
    
    typedef vector<int> vecint;
    typedef vector<vecint>  matriu;
    
    
    matriu temp;
    
    map<int, Pos> ciutats;
    
    Dir calc_direc(Pos posic, map<int, Pos> citys) {
        Pos propera;
        for (map<int,Pos>::const_iterator it = ciutats.begin(); it != ciutats.end(); it++) {
            if (it == ciutats.begin()) propera = it->second;
            else {
                if ((it->second).i < propera.i and (it->second).j < propera.j) propera = it->second;
            }
            
        }
        
        if (propera.i < posic.i) return LEFT;
        else if (propera.i > posic.i) return RIGHT;
        else if (propera.j < posic.j) return BOTTOM;
        else if (propera.j > posic.j) return TOP;
        else return NONE;
    }
    
    void move(int id) {
       
        Unit u = unit(id);
        Pos pos = u.pos;
        for (int d = 0; d != DIR_SIZE; ++d) {
            Dir direc = calc_direc(pos, ciutats);
            Pos npos = pos + direc;
            if (pos_ok(npos)) {
                execute(Command(id, direc));
                return;
            }
        }
    }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      
      if (round() == 0) { // Inicialitzem la matriu
          for (int i = 0; i < rows(); ++i)
              for (int j = 0; j < cols(); ++j)
                  if (cell(i,j).city_id != -1) {
                      Pos posic {i,j};
                      ciutats[cell(i,j).city_id] = posic;
                  }
      }
      
      vecint my_orks = orks(me());
      
      vecint perm = random_permutation(int(my_orks.size()));
      
      for (int k = 0; k < int(perm.size()); ++k){
          move(my_orks[perm[k]]);
      }
      
     
      
      
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
