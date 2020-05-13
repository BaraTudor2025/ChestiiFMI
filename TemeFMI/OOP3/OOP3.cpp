#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>

using namespace std;

/*
friend std::istream& operator>>(std::istream& in, )
{
    return in;
}
friend std::ostream& operator<<(std::ostream& out, )
{
    return out;
}
*/

class Examen {
protected:
    int numar = -1;
    int nota = -1;
    string materie = "";

	static std::unordered_map<std::string, int> map;

public:
    Examen() = default;

    Examen(std::string materie, int nota)
        :numar(getNumarAlMateriei(materie)),
        materie(materie),
        nota(nota)
    {}

    virtual ~Examen() = default;

    int getNumar() const { return numar; }
    int getNota() const { return nota; }
    const std::string& getMaterie() const { return materie; }

    virtual void read(std::istream& in) 
    {
        in >> materie;
        numar = getNumarAlMateriei(materie);
        in >> nota;
    }

    virtual void print(std::ostream& out) const = 0;

    static int getNumarAlMateriei(const std::string& materie)
    {
        static int s_nr = 1;
        if (map.count(materie) == 0)
            return map[materie] = s_nr++;
        else
            return map[materie];
    }

    static const std::string& getMateriById(int nr)
    {
        for (const auto& p : Examen::map)
            if (p.second == nr)
                return p.first;
        return std::string("");
    }
	
	friend std::istream& operator>>(std::istream& in, Examen& ex)
	{
        ex.read(in);
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const Examen& ex)
	{
        ex.print(out);
		return out;
	}
};

std::unordered_map<std::string, int> Examen::map;

class ExamenFinal final : public Examen {
    int puncteProiect = -1;

public:
    ExamenFinal() = default;

    ExamenFinal(std::string materie, int nota, int puncteProiect)
        :Examen(materie, nota), 
        puncteProiect(puncteProiect)
    {}

    ~ExamenFinal() = default;

    int getPuncteProiect() const { return puncteProiect; }

	virtual void read(std::istream& in) override
	{
		Examen::read(in);
        in >> puncteProiect;
    }

    virtual void print(std::ostream& out) const override 
    {
        out << "Examen final la " << materie << " cu nota " << nota << " la proiect " << puncteProiect;
    }
};

class Partial : public Examen {
    int notaOral = -1;
public:
    Partial() = default;
    Partial(std::string materie, int nota, int oral)
        : Examen(materie, nota),
        notaOral(oral)
    {}

    ~Partial() = default;

    int getNotaOral() const { return notaOral; }

	virtual void read(std::istream& in) override
	{
		Examen::read(in);
        in >> notaOral;
    }

    virtual void print(std::ostream& out) const override 
    {
        out << "Partial la " << materie << " cu nota in scris" << nota << " si nota la ora " << notaOral;
    }
};

class Quizz : public Examen {
    //int nrItemi= -1;

public:
    Quizz() = default;

    Quizz(std::string materie, int nota)
        : Examen(materie, nota)
    {}

    ~Quizz() = default;

    virtual void read(std::istream& in) override
    {
        Examen::read(in);
        //in >> nrItemi;
    }

    virtual void print(std::ostream& out) const override
    {
        out << "Quizz la " << materie << " cu nota " << nota;
    }
};

class Elev {
    const int nrMatricol;
    static int s_nrMatricol;
public:
    Elev() :nrMatricol(s_nrMatricol++) {}

    int getNrMatricol() const { return nrMatricol; }
};

int Elev::s_nrMatricol = 1;

template <typename T>
class CatalogIndividual {
    //int nrExamene;
    vector<T> examene;
    unordered_map<int, vector<Elev>> elevi;
    
public:
    CatalogIndividual() = default;

    CatalogIndividual& operator+=(T examen)
    {
        examene.push_back(std::move(examen));
        return *this;
    }
};

template <>
class CatalogIndividual<Quizz> {
    vector<Quizz> quizzes;
public:

};

std::unique_ptr<Examen> examenFactorie(int val)
{
    switch (val) {
    case 0:
        return std::unique_ptr<Partial>();
    case 1:
        return std::unique_ptr<ExamenFinal>();
    case 2:
        return std::unique_ptr<Quizz>();
    default:
        return nullptr;
    }
}

void menuElev()
{
    using std::cout;
    using std::cin;

	cout << "Introdu materia si nr matricol al restantierului: ";
    std::string materie;
	int nrMat;
    cin >> nrMat >> materie;
    
    cout << "Pentru partial scire nota scrisa, oral: ";
    int nota1, nota2;
    cin >> nota1 >> nota2;
    std::unique_ptr<Partial> partial(new Partial(materie, nota1, nota2));
    bool refacePartial = false;
    if (partial->getNota() < 4 && partial->getNotaOral() < 4) {
        refacePartial = true;
        cout << "Partialul se reface\n";
    }

    cout << "Pentru examenul final scrie nota scrisa, puncte proiect: ";
    cin >> nota1 >> nota2;
    std::unique_ptr<ExamenFinal> examen(new ExamenFinal(materie, nota1, nota2));

    float nota = (float)examen->getNota() * 0.6 + (float)examen->getPuncteProiect() * 0.4;
    if (!refacePartial) {
        float notaPartial = ((float) partial->getNota() + (float) partial->getNotaOral()) / 2;
        nota = (nota + notaPartial) / 2;
    }
    cout << "Nota este: " << nota << '\n';
    cout << "Vrei sa-ti maresti nota? y/n: ";
    char option;
    cin >> option;
    if (option == 'y' || option == 'Y') {
        
    }
    
	//auto examen = examenFactorie(option);
}


void menu()
{
    auto& in = cin;
    int option;
    do {
        in >> option;

    } while (option != -1);
}

int main()
{
    std::stringstream ss(R"(

)");

}
