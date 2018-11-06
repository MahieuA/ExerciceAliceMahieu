//
// Created by Alice Mahieu on 22/10/2018.
//

#include "network.h"

//Reset les valeurs et redimensionne la liste de noeuds, and values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
void Network::resize(const size_t& node) {
    values.resize(node);
    RNG.normal(values);
}



//Ajoute un lien bidirectionnel entre les noeuds, retourne true si les deux noeuds existent et que le lien n'existe pas
bool Network::add_link(const size_t& a, const size_t& b) {
    //on récupère les valeurs des neouds ayant pour index a et b
    //verifier que les noeuds exitent et remplir dans la map de facon bidirectionelle

    if ( (a<size()) and (b<size()) and (a!=b) ){

        for (std::multimap<size_t,size_t>::const_iterator it=links.begin(); it!=links.end(); ++it)
        {
            //si les liens existent deja on return false
            if( ( (it->first == a) and (it->second == b) ) or ( (it->first == b) and (it->second == a) ) ){
                return false;
            }
        }

        links.insert(std::pair<size_t,size_t>(a,b));
        links.insert(std::pair<size_t,size_t>(b,a));
        return true;

    } else {return false;}

}




//retourne le nombre de lien total de la map
size_t Network::random_connect(const double &mean) {

    links.clear();
    //on créer un tableau de taille size comportant le nombre de voisin de chaque noeud
    std::vector<int> number_links (size());
    //rempli le tableau avec des nombres aléatoires suivant la loi de poisson
    RNG.poisson(number_links, mean);
    size_t compteur = 0;

    //tableau contenant les indices
    std::vector<size_t> indice (size());

    for(size_t i=0; i<size();++i)
    {indice.push_back(i);}


    for (size_t i = 0; i < size(); ++i){
        RNG.shuffle(indice);

        for(int k = 0; k<number_links[i]; ++k){
            if( add_link(i,indice[k])){
                compteur = compteur + 1;
            } else {
                //si jamais le lien ne peut pas se créer, on re shuffle les indices pour créer au autre lien
                //et on retourne à l'étape précédente
                RNG.shuffle(indice);
                k=k-1;
            }

        }
    }

    return compteur;
}



//reset le nombre de noeuds qui on bien été reset le tableau donné en paramètres
size_t Network::set_values(const std::vector<double> &new_values) {
    if (size() >= new_values.size()){

        for(unsigned int i=0; i<=new_values.size();++i){
            values.erase(values.begin()+i);
            values[i]=new_values[i];
        }
        return new_values.size();

    } else if(size()< new_values.size()){
        values.clear();
        for(unsigned int i=0; i<=size(); ++i){
            values[i]=new_values[i];
        }
        return size();
    }

    return 0;
}



//donne le nombre de noeuds
size_t Network::size() const {
    return values.size();
}



//donne le nombre de noeuds associé au noeud _n (le nombre de link)
size_t Network::degree(const size_t &_n) const {
    return links.count(_n); //compte le nombre de lien associé a la value
}



//retourne la valeur associée au noeud _n dans le tableau de values
double Network::value(const size_t &_n) const {
    return values[_n];
}



//retourne un tableau contenant tous les noeuds triés de maniere décroissante?? (ne change pas les left values)
    std::vector<double> Network::sorted_values() const {
    std::vector<double> sortedvalues = values;
    sort(sortedvalues.begin(), sortedvalues.end(), std::greater<double>());

    return sortedvalues;
}



//retourne un tableau contenant les noeud liés au noeud _n
std::vector<size_t> Network::neighbors(const size_t &_n) const {

    std::vector<size_t>n_neighbors; //dans lequel on va stocker les noeuds liés, que l'on va retourner

    //je veux acceder a l'element n de ma multimap et rentrer toutes les valeurs associées dans mon tableau

    for (auto it=links.begin(); it!=links.end(); ++it){
        if(it->first == _n){
            n_neighbors.push_back(it->second);
        }
    }

    return n_neighbors;
}


