#include<string>
#include<map>
#include<queue>
#include<array>
#include<cmath>
#include<type_traits>
#include<iostream>

template<std::size_t s>
struct Puzzle final{
	std::array<char,s> board;
	int space;
	std::string path;

	explicit Puzzle()=default;
	Puzzle(const Puzzle&)=default;
	Puzzle(Puzzle&&)=default;
	Puzzle& operator=(const Puzzle&)=default;
	Puzzle& operator=(Puzzle&&)=default;
	
 	constexpr std::size_t size()const noexcept{return board.size();}	

	constexpr bool operator<(const Puzzle& other)const noexcept
	{
		std::size_t i=0;
		for(const auto& c:other){
			if(c==board[i]){
				++i;
				continue;
			}
			return board[i]>c;
		}
		return false;
	}

	const char& operator[](const std::size_t index)const&{return board[index];}
	char& operator[](const std::size_t index)&{return board[index];}
	char operator[](const std::size_t)&&=delete;
	
	typename decltype(board)::iterator begin()noexcept{return std::begin(board);}
	typename decltype(board)::iterator end()noexcept{return std::end(board);}
	typename decltype(board)::const_iterator begin()const noexcept{return std::begin(board);}
	typename decltype(board)::const_iterator end()const noexcept{return std::end(board);}

	struct is_puzzle_tag{};
};
 
template<class,class=std::void_t<>>
constexpr bool has_puzzle_tag=false;

template<class T>
constexpr bool has_puzzle_tag<T,std::void_t<typename T::is_puzzle_tag>> =true;

template<class Puzzle,std::enable_if_t<has_puzzle_tag<std::decay_t<Puzzle>>,std::nullptr_t> =nullptr>
constexpr bool is_target(Puzzle&& puzzle)noexcept
{
	for(std::size_t i{}; i<puzzle.size()-1; ++i){
		if(puzzle[i]!='1'+static_cast<signed>(i))return false;
	}
	return true;
} 

template<class Puzzle,std::enable_if_t<has_puzzle_tag<std::decay_t<Puzzle>>,std::nullptr_t> =nullptr>
void bfs(Puzzle&& pz)try
{
	using pz_type=std::decay_t<decltype(pz)>;
	const std::array<int,4> dx{{-1,0,1,0}},dy{{0,-1,0,1}};
	const std::array<std::string,4> results{{"down","right","up","left"}};
	const std::string linker=" -> ";
	const int sides=static_cast<int>(std::sqrt(pz.size()));

	std::queue<pz_type> que;
	std::map<pz_type,bool> pm;
	pz_type u,v;
	
	que.push(pz);
	pm[pz] = true;

    while(!que.empty()){
		u = que.front();
		que.pop();
		if(is_target(u)){
			u.path.erase(std::next(u.path.end(),-linker.size()),u.path.end());
			std::cout<<u.path<<std::endl;
			return;
		}
        
		int sx=u.space/sides,sy=u.space%sides;
		int tx{},ty{};
       
        for(std::size_t i=0; i<dx.size(); ++i){
			tx=sx+dx[i];
			ty=sy+dy[i];
			
			if(tx<0 or ty<0 or tx>=sides or ty>=sides)continue;
			
			v=u;
			v[u.space]=u[tx*sides+ty];
			v[tx*sides+ty]='x';
			v.space=tx*sides+ty;
			
			if(!pm[v]){
				pm[v] = true;
				v.path+=results[i];
				v.path+=linker;
				que.push(v);
			}
		}
	}
	std::cout<<"unsolvable"<< std::endl;
}catch(...){std::cerr<<"x not found"<<std::endl;}
 
int main()
{
	Puzzle<9> pz;

	std::size_t c=0;
	for(auto& i:pz){
		std::cin>>i;
		if(i=='x')pz.space=c;
		++c;
	}
   	bfs(std::move(pz));
}


