// Fichier d'include 
#include <iostream>
#include <iomanip>
#include <limits>
#include <array>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cctype>

// Déclaration des prototypes 
static void DisplayMainMenu();
static char InputLetter(const std::string& msg);
static bool LetterIsPresent(const char letter, const std::string& word);
static std::vector<std::string> LoadWordlist(const std::string& path);
static std::map<std::string, std::string> PeekWordFromDictionnary(const std::vector<std::string>& wordlist);
static void DiscoverLetterFromWordMap(std::map<std::string, std::string>&wordmap, const char letter);
static int ConsoleInputInteger(const std::string& msg, const int minInterval, const int maxInterval);
static void DrawHangingMan(const std::string& word, const int state);

// Constantes 
static constexpr std::string FILEPATH {"frwordlist"};

int main()
{
    bool bGameRun = true;

    while(bGameRun)
    {
        DisplayMainMenu();
        auto choice = ConsoleInputInteger("Select >", 1, 2);

        if(choice == 2)
        {
            std::cout << "...Bey..." << std::endl;
            bGameRun = false;
        }
        else
        {
            auto wordlist = LoadWordlist(FILEPATH);
            auto wordMap = PeekWordFromDictionnary(wordlist);
            bool gameOver = false;
            int hangingManState = 0;

            while(!gameOver)
            {
                char letter = InputLetter("Letter>");
         
                if(LetterIsPresent(letter, wordMap["word"]))
                {
                    DiscoverLetterFromWordMap(wordMap, letter);
                }
                else 
                {
                    hangingManState++;
                }
                DrawHangingMan(wordMap["hidden"], hangingManState);

                if(hangingManState == 7 || std::count(std::begin(wordMap["hidden"]), std::end(wordMap["hidden"]), '-') == 0)
                    gameOver = true;
             
            }

        }
    }

    return 0;
}

/**
 * Affiche le menu principale
*/
static void DisplayMainMenu()
{
    using std::cout;
    using std::endl;
    using std::setw;

    cout << setw(50) << "==========================" << endl;
    cout << setw(50) << "=       HANGING MAN      =" << endl;
    cout << setw(50) << "==========================" << endl;

	cout << " ___________.._______" << endl \
		 <<	"| .__________))______|" << endl \
		 <<	"| | / /      ||" << endl \
		 <<	"| |/ /       ||" << endl \
		 <<	"| | /        ||.-\'\'." << endl \
		 <<	"| |/         |/  _  \\" << endl \
		 <<	"| |          ||  `/,|" << endl \
		 <<	"| |          (\\`_.' " << endl \
		 <<	"| |         .-`--'." << endl \
		 <<	"| |        /Y . . Y\\\\" << endl \
		 <<	"| |       // |   |  \\\\" << endl \
		 <<	"| |      //  | . |   \\\\" << endl \
		 <<	"| |     ')   |   |    (`" << endl \
		 <<	"| |          || ||" << endl \
		 <<	"| |          || ||" << endl \
		 <<	"| |          || ||" << endl \
		 <<	"| |          || ||" << endl \
		 <<	"| |         / | | \\" << endl \
		 <<	"__________| `-  `-  |_____|" << endl \
		 <<	"| |       \\ \\         | |" << endl \
		 <<	"| |        \\ \\        | |" << endl  \
		 <<	": :         \\ \\       : :  " << endl; \
    cout << endl;
    cout << "(1) Start a new game" << endl;
    cout << "(2) Leave application" << endl; 
}
/**
 * Demande a l'utilisateur une lettre au clavier
*/
static char InputLetter(const std::string& msg)
{
    std::string buffer;
    bool isChar = false;
    do 
    {
        isChar = true;
        std::cout << msg;
        std::cin >> buffer;

        if(!std::isalpha(buffer[0]))
        {
            std::cout << "Please enter a letter !" << std::endl;
            isChar = false;
        }
 
    }while(!isChar);
     
    return std::tolower(buffer[0]);
}
/**
 * Vérifie si la lettre est présente dans le mot 
*/
static bool LetterIsPresent(const char letter, const std::string& word)
{
    for(char c : word)
    {
        if(c == letter)
            return true;
    }

    return false;
}
/**
 * Lit le fichier wordlist passez en paramètre 
*/
static std::vector<std::string> LoadWordlist(const std::string& path)
{
    std::fstream file{path, std::ios::in};
    std::vector<std::string> words;

    if(file.is_open())
    {
        while(!file.eof())
        {
            std::string word;
            std::getline(file, word);
            words.push_back(word);
        }
    }
    return words;
}
/**
 * Récupère un mot aléatoirement depuis la wordlist 
*/
static std::map<std::string, std::string> PeekWordFromDictionnary(const std::vector<std::string>& wordlist)
{
    // Génère un nombre aléatoire entre 0 et le nombre d'éléments du wordlist 
    std::random_device rd;
    auto wordIndex = std::uniform_int_distribution<int>(0, wordlist.size())(rd);
    auto word = wordlist[wordIndex];
    std::for_each(std::begin(word), std::end(word), [](char& c) -> void { c = std::tolower(c);}); // mettre le mot en miniscule 

    // Créer un map avec le mot cacher et le mot en clair 
    std::map<std::string, std::string> wordMap;
    wordMap["word"] = word;
    wordMap["hidden"] = std::string(word.size(), '-');
    return wordMap;
}
static int ConsoleInputInteger(const std::string& msg, const int minInterval, const int maxInterval)
{
    int data = 0;  
    bool bInputOk = false;

    while(!bInputOk)
    {
        bInputOk = true;
        std::cout << msg;
        std::cin >> data;
         
        if(!std::cin.good() || data < minInterval || data > maxInterval)
        {   
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "You need to input a number between " << minInterval << " and " << maxInterval << std::endl;
            bInputOk = false;
        }
    }
 
  
    return data;
}
// Affiche a l'écran l'état du pendu 
static void DrawHangingMan(const std::string& word, const int state)
{
    using std::cout;
    using std::endl;
    cout << "The hidden word: " << word << std::endl;
    switch(state)
    {
        case 1:
            cout <<  "+---+" << endl \
                 <<  "|   |" << endl \
                 <<  "    |" << endl \
                 <<  "    |" << endl \ 
                 <<  "    |" << endl \
                 <<  "    |" << endl \
                 <<  "=========" << endl;
            break;
        case 2:
            cout <<  "+---+" << endl \
                 <<  "|   |" << endl \
                 <<  "o   |" << endl \
                 <<  "    |" << endl \ 
                 <<  "    |" << endl \
                 <<  "    |" << endl \
                 <<  "=========" << endl;
            break;
        case 3:
            cout <<  "+---+" << endl \
                 <<  "|   |" << endl \
                 <<  "o   |" << endl \
                 <<  "|   |" << endl \ 
                 <<  "    |" << endl \
                 <<  "    |" << endl \
                 <<  "=========" << endl;
            break;
        case 4:
            cout <<  " +---+" << endl \
                 <<  " |   |" << endl \
                 <<  " o   |" << endl \
                 <<  "/|   |" << endl \ 
                 <<  "     |" << endl \
                 <<  "     |" << endl \
                 <<  " =========" << endl;
            break;
        case 5:
            cout <<  " +---+" << endl \
                 <<  " |   |" << endl \
                 <<  " o   |" << endl \
                 <<  "/|\\  |" << endl \ 
                 <<  "     |" << endl \
                 <<  "     |" << endl \
                 <<  " =========" << endl;
            break;
        case 6:
            cout <<  " +---+" << endl \
                 <<  " |   |" << endl \
                 <<  " o   |" << endl \
                 <<  "/|\\  |" << endl \ 
                 <<  "/    |" << endl \
                 <<  "     |" << endl \
                 <<  " =========" << endl;
            break;
        case 7:
            cout <<  " +---+" << endl \
                 <<  " |   |" << endl \
                 <<  " o   |" << endl \
                 <<  "/|\\  |" << endl \ 
                 <<  "/ \\  |" << endl \
                 <<  "     |" << endl \
                 <<  " =========" << endl;
            break;
    }
}
static void DiscoverLetterFromWordMap(std::map<std::string, std::string>&wordmap, const char letter)
{
    for(auto i = 0; i < wordmap["word"].size(); i++)
    {
        if(wordmap["word"][i] == letter)
        {
            wordmap["hidden"][i] = letter;
        }
    }
}
