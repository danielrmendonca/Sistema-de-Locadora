
import Header from '../components/Header';
import MovieShelf from '../components/MovieShelf';

const Index = () => {
  return (
    <div className="min-h-screen bg-gray-200 font-verdana">
      <Header />
      
      <main className="container mx-auto px-4 py-8">
        <div className="bg-white shadow-lg p-6 mb-8">
          <div className="text-center space-y-4">
            <h1 className="text-4xl font-bold text-blue-900">
              BEM-VINDO À ERA DOURADA DO CINEMA!
            </h1>
            <p className="text-lg text-gray-700 max-w-2xl mx-auto">
              Reviva a nostalgia dos anos 2000 com nossa seleção especial de filmes clássicos. 
              Alugue seus favoritos e tenha uma noite inesquecível!
            </p>
            <div className="flex justify-center space-x-4 text-sm font-bold">
              <span className="bg-blue-100 text-blue-800 px-3 py-1">
                QUALIDADE DVD
              </span>
              <span className="bg-gray-100 text-gray-800 px-3 py-1">
                ENTREGA INSTANTÂNEA
              </span>
              <span className="bg-blue-100 text-blue-800 px-3 py-1">
                CLÁSSICOS ATEMPORAIS
              </span>
            </div>
          </div>
        </div>

        <MovieShelf />
      </main>

      <footer className="bg-gray-800 mt-12 py-8">
        <div className="container mx-auto px-4 text-center">
          <p className="text-blue-200 font-bold text-lg mb-2">
            LOCADORA MILK - A Nostalgia Nunca Sai de Moda
          </p>
          <p className="text-gray-300 text-sm">
            © 2000-2024 • Feito com 💜 para os amantes do cinema
          </p>
        </div>
      </footer>
    </div>
  );
};

export default Index;
