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
        cerr << "------" << endl;
        cerr << "ork a " << posic.i << ' ' << posic.j << endl;
        
        Pos propera; // Posició de la ciutat més propera al ork
        double dist;
        
        for (map<int,Pos>::const_iterator it = citys.begin(); it != citys.end(); it++) {
            if (it == citys.begin()) propera = it->second;
            else {
                if (abs((it->second).i - posic.i) < abs(propera.i - posic.i) and abs((it->second).j - posic.j) < abs(propera.j - posic.j)){
                    propera = it->second;
                }
            }
            
        }
        
        cerr << "la ciutat més propera és " << propera.i << " " << propera.j << endl;
        
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
            // TEST
            cerr << "ork " << id << " va cap a " << direc << endl;
            
  
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
              if (i < 10) cout << ' ';
              cout << i;
              for (int j = 0; j < cols(); ++j) {
                  
                  Cell c = cell(i,j);
                  if (c.city_id != -1) cout << 'C';
                  else cout << '.';
       
                  if (c.city_id != -1) {
                      Pos posic;
                      posic.i = i;
                      posic.j = j;
                      
                      ciutats[cell(i,j).city_id] = posic;
                  }
              }
              cout << endl;
          }
          
      }
      
      vecint my_orks = orks(me());
      
    
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
