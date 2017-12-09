#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME mcasellas_v2


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
    
    
   
    typedef vector< pair<int, int> > Adj; // Distance - node
    typedef vector<Adj> Graf;
    
    Graf mapa;
    
    map <int, stack<int> > ork_rec; // stack de recorreguts
    
    void dijkstra(int u, int v, stack<int>& result) {
        int n = int(mapa.size());
        vector<int> distance(n, numeric_limits<int>::max()); // Infinite
        distance[u] = 0;
        vector<bool> visited(n, false);
        vector<int> whereFrom(n, -1);
        priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pQ;
        pQ.push({0, u});
        while (!pQ.empty() && pQ.top().second != v) {
            int new_node = pQ.top().second;
            pQ.pop();
            if (!visited[new_node]) {
                visited[new_node] = true;
                for (int i = 0; i < mapa[new_node].size(); ++i) {
                    pair<int, int> aux = mapa[new_node][i];
                    if (distance[new_node] + aux.first < distance[aux.second]) {
                        distance[aux.second] = distance[new_node] + aux.first;
                        whereFrom[aux.second] = new_node;
                        pQ.push({distance[aux.second], aux.second});
                    }
                }
            }
        }
        
        while (whereFrom[v] != -1) {
            //cerr << "RESULT " << v/70 << ',' << v%70 << endl;
            result.push(v);
            v = whereFrom[v];
        }
        result.push(v);
       
    }
    
    
    
    void calc_direc(Pos act, int id) {
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
        //cerr << "id  " << id << endl;
        //cerr << "act  " << act.i << ',' << act.j << endl;
        //cerr << "propera  " << propera.i << ',' << propera.j << endl;
        
        dijkstra(act.i*cols()+act.j, propera.i*cols()+propera.j, ork_rec[id]);
        
       
        
    }
    
    
    void move(int id) {
        //cerr << "ork " << id << endl;
        Unit u = unit(id); // Obtenim la unitat
        Pos act = u.pos; // La posició actual de la unitat
        
        calc_direc(act,id);
        if (!(ork_rec[id]).empty())  (ork_rec[id]).pop();
        
        if (!(ork_rec[id]).empty()) {
           
            //cerr << "ork " << id << endl;
            int num = (ork_rec[id]).top();
            (ork_rec[id]).pop();
            
            Dir direc = cap_on({num/cols(), num%cols()}, act);
            
            Pos valid = act + direc;
            if (cell(valid).unit_id == -1 or (unit(cell(valid).unit_id).health <= u.health and unit(cell(valid).unit_id).player != me())){
                
                execute(Command(id, direc));
                
                return;
            }
            else {
                Dir nova;
                while ((nova = Dir(random(0, 4))) == direc);
                execute(Command(id, nova));
                return;
            }
            
        }
        
        else return;
    }
    
    /**
     * Play method, invoked once per each round.
     */
    virtual void play () {
      
        
        if (round() == 0) { // Inicialitzacions
            
            
            
            mapa = Graf(rows()*rows());
        
            for (int i = 0; i < rows(); ++i) { // Busquem la ciutat més propera
                for (int j = 0; j < cols(); ++j) {
                    Cell c = cell(i,j);
                    
                    if (c.city_id != -1) ciutats[cell(i,j).city_id].push_back({i,j}); // Carreguem totes les ciutats
                    if (c.path_id != -1) camins[cell(i,j).path_id].push_back({i,j}); // Carreguem tots els camins
                    if (c.unit_id != -1) pos_orks[cell(i,j).unit_id] = {i,j};
                    
                    if (cell(i,j).type != 0) {
                        if (j < cols()-1 and cell(i,j+1).type != 0) {
                            mapa[i*cols()+j].push_back({cost(cell(i,j+1).type), i*cols()+j+1});
                            mapa[i*cols()+j+1].push_back({cost(cell(i,j).type), i*cols()+j});
                        }
                        if (i < rows()-1 and cell(i+1,j).type != 0) {
                            mapa[i*cols()+j].push_back({cost(cell(i+1,j).type), (i+1)*cols()+j });
                            mapa[(i+1)*cols()+j].push_back({cost(cell(i,j).type), i*cols()+j });
                        }
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
