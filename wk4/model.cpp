#include <iostream>
#include <fstream>
#include <vector>
#include"C:\src\json\single_include\nlohmann\json.hpp"

using json = nlohmann::json;

class state
{
    public:
        double x, v, w;
        state(double x, double v, double w): x(x), v(v), w(w) {};
        state operator + (state other)
        {
            return state {this->x + other.x, this->v + other.v, this->w};
        }
        state operator * (double r)
        {
            return state {r * this->x, r * this->v, this->w};
        }
        
};

state f(state y, double x)
{
    state t = {y.v, -y.w * y.w * y.x, y.w};
    return t;
}

class Solver
{
    private:
        std::vector <state> data;
        double w, x0, v0, L, h;
        int N;
        const std::string model, file;
        void add_step()
        {
            state y = this->data[data.size()-1];
            if (model == "Euler")
            {
                data.push_back(y + f(y, y.x) * h);
            }
            if (model == "Heun")
            {
                state k1 = f(y, y.x);
                state k2 = f(y + k1 * h, y.x + k1.x);
                data.push_back(state {y + (k1 + k2) * 0.5 * h});
            }
            if (model == "RK45")
            {
                state k1 = f(y, y.x);
                state k2 = f(y + k1 * 0.5 * h, y.x + 0.5 * h);
                state k3 = f(y + k2 * 0.5 * h, y.x + 0.5 * h);
                state k4 = f(y + k3 * h, y.x + h);
                data.push_back(y + (k1 + k2 * 2 + k3 * 2 + k4) * (h / 6));
            }
        }
    public:
        Solver(double w, double x0, double v0, double L, double h, const std::string model, const std::string file): 
            w(w), x0(x0), v0(v0), L(L), h(h), model(model), file(file)
        {
            data.push_back(state {this->x0, this->v0, this->w});
            this->N = (int)(this->L / this->h);
        };
        void solve()
        {
            for (unsigned i = 0; i < this->N; ++i)
            {
                (*this).add_step();
                if (i == static_cast<int>(static_cast<double>(this->N)*0.1)-1) std::cout << "10\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.2)-1) std::cout << "20\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.3)-1) std::cout << "30\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.4)-1) std::cout << "40\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.5)-1) std::cout << "50\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.6)-1) std::cout << "60\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.7)-1) std::cout << "70\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.8)-1) std::cout << "80\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*0.9)-1) std::cout << "90\% solved\n";
                if (i == static_cast<int>(static_cast<double>(this->N)*1)-1) std::cout << "100\% solved\n";
            }
            std::cout << "status: solved successfully\n\n";
        }
        void print()
        {
            std::cout << std::scientific <<
                "type: Solver\nparameters:\nmodel\t\t" + this->model +
                "\ninit. coord\t" << this->x0 <<
                "\ninit. velocity\t" << this->v0 <<
                "\nomega\t\t" << this->w <<
                "\nmod. interspace\t" << this->L <<
                "\nstep of count\t"<< this->h << std::endl << std::endl;
        }
        void write()
        {
            std::cout << "collecting files\n";
            std::ofstream out(this->file);
            out << "x,v,w,L,h\n";
            for (unsigned i = 0; i < this->N; ++i)
            {
                out << 
                    this->data[i].x << ',' << 
                    this->data[i].v << ',' << 
                    this->w << ',' <<
                    this->L << ',' <<
                    this->h << "\n";
            }
            out.close();
            std::cout << "data has been written to " + this->file << std::endl << std::endl;
        }
        void clear()
        {
            this->data.clear();
        }
};




int main()
{
    std::ifstream in("config_file.json");
    json config = json::parse(in);
    in.close();

    std::vector <Solver> Solvers;

    std::cout << "Entries number: " << config["entries"].size() << std::endl << std::endl;


    for (unsigned i = 0; i < config["entries"].size(); ++i)
    { // w(w), x0(x0), v0(v0), h(h), L(L), model(model)
        Solvers.push_back( Solver {
            config["entries"][i]["omega"],
            config["entries"][i]["initial coordinate"],
            config["entries"][i]["initial velocity"],
            config["entries"][i]["modeling interspace"],
            config["entries"][i]["step of modeling"],
            config["entries"][i]["model"],
            config["file names"][i]["name"]
        });
    }
    
    for (unsigned i = 0; i < Solvers.size(); ++i) {Solvers[i].print(); Solvers[i].solve();}
    for (unsigned i = 0; i < Solvers.size(); ++i) Solvers[i].write();
    
    std::cout << "end of modeling";

    return 0;
}