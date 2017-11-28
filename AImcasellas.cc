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
    
    double distancia (Pos a, Pos b){
        return sqrt(abs(a.i-b.i)*abs(a.i-b.i) + abs(a.j-b.j)*abs(a.j-b.j));
    }
    
    Dir calc_direc(Pos posic, map<int, Pos> citys) {
        Pos propera; // Posició de la ciutat més propera a l'ork
        for (map<int,Pos>::const_iterator it = citys.begin(); it != citys.end(); it++) {
            if (it == citys.begin()) propera = it->second;
            else {
                Pos cand = it->second;
                if (distancia(posic, it->second) < distancia(posic, propera)) propera = it->second;
                
            }
            
        }
        
        if (propera.i < posic.i) return TOP;
        else if (propera.i > posic.i) return BOTTOM;
        else if (propera.j < posic.j) return LEFT;
        else if (propera.j > posic.j) return RIGHT;
        else return NONE;
    }
    
    void move(int id) {
        Unit u = unit(id); // Obtenim la unitat
        Pos pos = u.pos; // La posició actual de la unitat
            Dir direc = calc_direc(pos, ciutats);
            execute(Command(id, direc));
            return;
    }
    
    
    

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      
      if (round() == 0) { // Inicialitzacions
          cout << "  ";
          for (int k = 0; k < 7; k++) {
              for (int l = 0; l < 10; l++) {
                  cout << k;
              }
          }
          cout << endl;
          cout << "  ";
          for (int k = 0; k < 7; k++) {
              for (int l = 0; l < 10; l++) {
                  cout << l;
              }
          }
          cout << endl;
          
          
          for (int i = 0; i < rows(); ++i) { // Busquem la ciutat més propera
              for (int j = 0; j < cols(); ++j) {

                  Cell c = cell(i,j);
 
                  if (c.city_id != -1) {
                      Pos posic;
                      posic.i = i;
                      posic.j = j;
                      
                      ciutats[cell(i,j).city_id] = posic;
                  }
              }
          }
          
      }
      
      vecint my_orks = orks(me());
      
      // Moure tots els orks
      for (int k = 0; k < my_orks.size(); ++k){
          cerr << "es mou " << my_orks[k] << endl;
          move(my_orks[k]); // movem ork a ork
         
      }
      
     
      
      
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
