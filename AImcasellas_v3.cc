#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME mcasellas_v3

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
    
    Graf G;
    
    map <int, stack<int> > ork_rec; // stack de recorreguts
    
void dijkstra(int u, int v, stack<int>& result) {
        int n = int(G.size());
        vector<int> distance(n, numeric_limits<int>::max()); // Infinite
        distance[u] = 0;
        vector<bool> d(n, false);
        vector<int> p(n, -1);
        priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pQ;
        pQ.push({0, u});
        while (!pQ.empty() && pQ.top().second != v) {
            int intermig = pQ.top().second;
            pQ.pop();
            if (!d[intermig]) {
                d[intermig] = true;
                for (int i = 0; i < G[intermig].size(); ++i) {
                    pair<int, int> adjacent = G[intermig][i];
                    if (distance[intermig] + adjacent.first < distance[adjacent.second]) {
                        distance[adjacent.second] = distance[intermig] + adjacent.first;
                        p[adjacent.second] = intermig;
                        pQ.push({distance[adjacent.second], adjacent.second});
                    }
                }
            }
        }
        
        while (p[v] != -1) {
            //cerr << v/cols() << ',' << v%cols() << endl;
            result.push(v);
            v = p[v];
        }
    }
    
    void calc_direc(Pos act, int id) {
        Pos propera_ciutat {numeric_limits<int>::max(),numeric_limits<int>::max()}; // Inicialitzem la ciutat més propera a l'ork amb un valor infinit
        Pos proper_cami {numeric_limits<int>::max(),numeric_limits<int>::max()};
        for (map<int,vector<Pos>>::const_iterator it = camins.begin(); it != camins.end(); it++) {
            for (int i = 0; i < (it->second).size(); i++){
                if (path_owner(it->first) != me() and distancia(act, it->second[i]) < distancia(act, proper_cami) and act != it->second[i]) proper_cami = it->second[i];
            }
        }
        
        for (map<int,vector<Pos>>::const_iterator it = ciutats.begin(); it != ciutats.end(); it++) {
            for (int i = 0; i < (it->second).size(); i++){
                if (city_owner(it->first) != me() and it->second[i] != act and distancia(act, it->second[i]) < distancia(act, propera_ciutat) and act != it->second[i]) propera_ciutat = it->second[i];
            }
        }
        
        if (propera_ciutat == Pos(numeric_limits<int>::max(),numeric_limits<int>::max())) propera_ciutat = act;
        
        Pos propera {numeric_limits<int>::max(),numeric_limits<int>::max()};
        
        if (distancia(propera_ciutat, act) < distancia(proper_cami, act)) propera = propera_ciutat;
        else propera = proper_cami;

        dijkstra(act.i*cols()+act.j, propera.i*cols()+propera.j, ork_rec[id]);
        
        
    }
    
    
    void move(int id) {
        Unit u = unit(id); // Obtenim la unitat
        Pos act = u.pos; // La posiciĂł actual de la unitat
        
        if (ork_rec[id].empty()) calc_direc(act,id);
        
        if (!ork_rec[id].empty()) {
            
            
            int num = (ork_rec[id]).top();
            (ork_rec[id]).pop();
            
            Dir direc = cap_on({num/cols(), num%cols()}, act);
            
            Pos valid = act + direc;
            
            if (cell(valid).unit_id == -1 or unit(cell(valid).unit_id).health <= u.health){
                
                execute(Command(id, direc));
                
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
            
            G = Graf(rows()*rows());
            
            for (int i = 0; i < rows(); ++i) { // Busquem la ciutat mĂŠs propera
                for (int j = 0; j < cols(); ++j) {
                    Cell c = cell(i,j);
                    
                    if (c.city_id != -1) ciutats[cell(i,j).city_id].push_back({i,j}); // Carreguem totes les ciutats
                    if (c.path_id != -1) camins[cell(i,j).path_id].push_back({i,j}); // Carreguem tots els camins
                    if (c.unit_id != -1) pos_orks[cell(i,j).unit_id] = {i,j};
                    
                    if (cell(i,j).type != 0) {
                        if (j < cols()-1 and cell(i,j+1).type != 0) {
                            G[i*cols()+j].push_back({cost(cell(i,j+1).type), i*cols()+j+1});
                            G[i*cols()+j+1].push_back({cost(cell(i,j).type), i*cols()+j});
                        }
                        if (i < rows()-1 and cell(i+1,j).type != 0) {
                            G[i*cols()+j].push_back({cost(cell(i+1,j).type), (i+1)*cols()+j });
                            G[(i+1)*cols()+j].push_back({cost(cell(i,j).type), i*cols()+j });
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
