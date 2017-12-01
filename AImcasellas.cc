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

    map<int, vector<Pos>> ciutats;
    map<int, vector<Pos>> camins;
    map<int, Pos> pos_orks;

    
    double distancia (Pos a, Pos b){
        return sqrt(abs(a.i-b.i)*abs(a.i-b.i) + abs(a.j-b.j)*abs(a.j-b.j));
    }
    
    Dir cap_on(Pos propera, Pos posic) {
        if (propera.j < posic.j) return LEFT;
        else if (propera.j > posic.j) return RIGHT;
        else if (propera.i < posic.i) return TOP;
        else if (propera.i > posic.i) return BOTTOM;
        else return NONE;
    }
    
    Dir calc_direc(Pos act) {
        Pos propera {5000,5000}; // Inicialitzem la ciutat més propera a l'ork
        for (map<int,vector<Pos>>::const_iterator it = camins.begin(); it != camins.end(); it++) {
            for (int i = 0; i < (it->second).size(); i++){
                if (path_owner(it->first) != me() and distancia(act, it->second[i]) < distancia(act, propera)) propera = it->second[i];
            }
            
        }
        for (map<int,vector<Pos>>::const_iterator it = ciutats.begin(); it != ciutats.end(); it++) {
            for (int i = 0; i < (it->second).size(); i++){
                if (city_owner(it->first) != me() and it->second[i] != act and distancia(act, it->second[i]) < distancia(act, propera)) propera = it->second[i];
            }

        }
        
        return cap_on(propera, act);
        
    }
    
    void move(int id) {
        Unit u = unit(id); // Obtenim la unitat
        Pos act = u.pos; // La posició actual de la unitat
        
        Dir direc = calc_direc(act);
        Pos valid = act + direc;
        if (cell(valid).unit_id == -1 or unit(cell(valid).unit_id).health <= u.health){
        execute(Command(id, direc));
        return;
        }
    }
    

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      
      if (round() == 0) { // Inicialitzacions
          
          for (int i = 0; i < rows(); ++i) { // Busquem la ciutat més propera
              for (int j = 0; j < cols(); ++j) {

                  Cell c = cell(i,j);
 
                  if (c.city_id != -1) ciutats[cell(i,j).city_id].push_back({i,j}); // Carreguem totes les ciutats
                  if (c.path_id != -1) camins[cell(i,j).path_id].push_back({i,j}); // Carreguem tots els camins
                  if (c.unit_id != -1) pos_orks[cell(i,j).unit_id] = {i,j};
              }
          }
          
      }
    
      vector<int> my_orks = orks(me());
      vector<int> perm = random_permutation(my_orks.size());
      
      // Moure tots els orks
      for (int k = 0; k < my_orks.size(); ++k) move(my_orks[perm[k]]); // movem ork a ork
         
      
      
     
      
      
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
