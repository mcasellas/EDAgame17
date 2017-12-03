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
    
    typedef pair<int, Pos> fila;        // Cost, node
    typedef vector< vector<fila> > graf;  // graf amb pesos
    
    graf G;
    
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
    
    
    void dijkstra(Pos inicial, Pos objectiu) {
       
        map <Pos, int> distancia;
        
        distancia[inicial] = 0;
        
        map <Pos, bool> visitats;
        
        map <Pos, int> whereFrom;
        
        priority_queue< fila, vector< fila >, greater< fila > > pQ;
        
        pQ.push({0, inicial});
        
        while (!pQ.empty() && pQ.top().second != objectiu) {
            
            Pos new_node = pQ.top().second;
            
            if (distancia.find(new_node) == distancia.end()) distancia[new_node] = numeric_limits<int>::max(); // Assignem infinit
            
            pQ.pop();
            if (visitats.find(new_node) == visitats.end()) {
                visitats[new_node] = true;
                for (int i = 0; i < G[new_node.j].size(); ++i) {
                    
                    pair<int, Pos> aux = G[new_node.i][i];
                    if (distancia[new_node] + aux.first < distancia[aux.second]) {
                        distancia[aux.second] = distancia[new_node] + aux.first;
                        whereFrom[new_node];
                        
                        pQ.push({distancia[aux.second], aux.second});
                    }
                }
            }
        }
        if (pQ.empty()) {
            cout << "no path from " << inicial.i << ',' << inicial.j << " to " << objectiu.i << ',' << objectiu.j << endl;
            return;
        }
        stack<Pos> result;
        
        while (whereFrom.find(objectiu) != whereFrom.end()) {
            result.push(objectiu);
            objectiu = whereFrom.find(objectiu)->first;
        }
        
        result.push(objectiu);
        
        cout << result.top();
        
        result.pop();
        
        while (!result.empty()) {
            cout << " " << result.top();
            result.pop();
        }
        cout << endl;
    }
    

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      
      if (round() == 0) { // Inicialitzacions
          
          for (int i = 0; i < rows(); ++i) { // Busquem la ciutat més propera
              for (int j = 0; j < cols(); ++j) {
                  /*
                  
                  if (c.city_id != -1) ciutats[cell(i,j).city_id].push_back({i,j}); // Carreguem totes les ciutats
                  if (c.path_id != -1) camins[cell(i,j).path_id].push_back({i,j}); // Carreguem tots els camins
                  if (c.unit_id != -1) pos_orks[cell(i,j).unit_id] = {i,j};
                */
                if (cell(i,j).type != 0) {
                    if (j < cols()-1 and cell(i,j+1).type != 0) G[i].push_back({cost(cell(i,j+1).type), {i,j+1}});
                    if (i < rows()-1 and cell(i+1,j).type != 0) G[i].push_back({cost(cell(i+1,j).type), {i+1,j}});
                  }
                  
                }
          }
          
      }
    
      vector<int> my_orks = orks(me());
      vector<int> perm = random_permutation(int(my_orks.size()));
      
      // Moure tots els orks
      for (int k = 0; k < my_orks.size(); ++k) move(my_orks[perm[k]]); // movem ork a ork
         
      
      
     
      
      
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
