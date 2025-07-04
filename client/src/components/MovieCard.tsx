import { Film } from 'lucide-react';

interface Movie {
  id: number;
  title: string;
  year: number;
  genre: string;
  price: number;
  available: boolean;
  image?: string;
}

interface MovieCardProps {
  movie: Movie;
  onRent: (movieId: number) => void;
}

const MovieCard = ({ movie, onRent }: MovieCardProps) => {
  return (
    <div className="bg-gray-100 p-4 shadow-lg">
      <div className="bg-gray-800 aspect-[3/4] mb-3 flex items-center justify-center">
        {movie.image ? (
          <img src={movie.image} alt={movie.title} className="w-full h-full object-cover" />
        ) : (
          <Film size={48} className="text-gray-400" />
        )}
      </div>
      
      <div className="space-y-2">
        <h3 className="font-bold text-lg text-gray-900 leading-tight">
          {movie.title}
        </h3>
        <p className="text-sm text-gray-600">
          {movie.year} • {movie.genre}
        </p>
        <div className="flex items-center justify-between">
          {/* A cor do preço foi alterada aqui */}
          <span className="text-xl font-bold text-[#A8109B]">
            R$ {movie.price.toFixed(2)}
          </span>
          <span className={`text-sm font-bold px-2 py-1 ${
            movie.available 
              ? 'bg-[#F2CFEF] text-[#610a59]'
              : 'bg-red-100 text-red-800'
          }`}>
            {movie.available ? 'Disponível' : 'Alugado'}
          </span>
        </div>
        
        <button
          onClick={() => onRent(movie.id)}
          className={`w-full py-2 px-4 font-bold text-white ${
            movie.available
              ? 'bg-[#610A59] hover:bg-[#A8109B]'
              : 'bg-gray-600 hover:bg-gray-500'
          }`}
        >
          {movie.available ? 'ALUGAR AGORA!' : 'DEVOLUÇÃO'}
        </button>
      </div>
    </div>
  );
};

export default MovieCard;