import { useState } from 'react';
import MovieCard from './MovieCard';

interface Movie {
  id: number;
  title: string;
  year: number;
  genre: string;
  price: number;
  available: boolean;
  image?: string;
}

const MovieShelf = () => {
  const [movies, setMovies] = useState<Movie[]>([
    {
      id: 1,
      title: "Matrix",
      year: 1999,
      genre: "Ficção Científica",
      price: 3.50,
      available: true,
      image: 'https://br.web.img2.acsta.net/medias/nmedia/18/91/08/82/20128877.JPG'
    },
    {
      id: 2,
      title: "Titanic",
      year: 1997,
      genre: "Romance/Drama",
      price: 4.00,
      available: false,
      image: 'https://upload.wikimedia.org/wikipedia/pt/2/22/Titanic_poster.jpg'
    }, 
    {
      id: 3,
      title: "Jurassic Park",
      year: 1993,
      genre: "Aventura",
      price: 3.00,
      available: true,
      image: 'https://upload.wikimedia.org/wikipedia/pt/e/e7/Jurassic_Park_poster.jpg'
    },
    {
      id: 4,
      title: "Star Wars: Episódio I",
      year: 1999,
      genre: "Ficção Científica",
      price: 3.50,
      available: true,
      image: 'https://upload.wikimedia.org/wikipedia/pt/3/30/Star_Wars_Epis%C3%B3dio_1_Amea%C3%A7a_Fantasma.jpg'
    },
    {
      id: 5,
      title: "American Beauty",
      year: 1999,
      genre: "Drama",
      price: 3.50,
      available: true,
      image: 'https://upload.wikimedia.org/wikipedia/pt/9/99/AmericanBeautyPoster.jpg'
    },
    {
      id: 6,
      title: "O Rei Leão",
      year: 1994,
      genre: "Animação",
      price: 2.50,
      available: false,
      image: 'https://ingresso-a.akamaihd.net/img/cinema/cartaz/4116-cartaz.jpg'
    },
    {
      id: 7,
      title: "Forrest Gump",
      year: 1994,
      genre: "Drama",
      price: 3.00,
      available: true,
      image: 'https://upload.wikimedia.org/wikipedia/pt/c/c0/ForrestGumpPoster.jpg'
    },
    {
      id: 8,
      title: "Pulp Fiction",
      year: 1994,
      genre: "Crime",
      price: 3.50,
      available: true,
      image: 'https://upload.wikimedia.org/wikipedia/pt/8/82/Pulp_Fiction_cover.jpg'
    }
  ]);

  const handleRent = (movieId: number) => {
    setMovies(prevMovies =>
      prevMovies.map(movie =>
        movie.id === movieId
          ? { ...movie, available: !movie.available }
          : movie
      )
    );
  };

  const rentedCount = movies.filter(movie => !movie.available).length;

  return (
    <div className="space-y-8">
      <section>
        <div className="bg-gray-700 p-4 mb-6 shadow-lg">
          <h2 className="text-2xl font-bold text-white text-center">
            FILMES DISPONÍVEIS PARA LOCAÇÃO
          </h2>
          <p className="text-center text-gray-300 font-bold mt-1">
            {movies.filter(movie => movie.available).length} filmes esperando por você!
          </p>
          <p className="text-center text-yellow-300 font-bold mt-2">
            {rentedCount} filmes alugados no momento
          </p>
        </div>
        
        <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-6">
          {movies.map(movie => (
            <MovieCard
              key={movie.id}
              movie={movie}
              onRent={handleRent}
            />
          ))}
        </div>
      </section>
    </div>
  );
};

export default MovieShelf;