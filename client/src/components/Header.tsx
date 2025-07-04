
import { Film } from 'lucide-react';

const Header = () => {
  return (
    <header className="bg-blue-900 shadow-lg">
      <div className="container mx-auto px-4 py-6">
        <div className="flex items-center justify-between">
          <div className="flex items-center space-x-3">
            <div className="bg-gray-200 p-2">
              <Film size={32} className="text-blue-900" />
            </div>
            <div>
              <h1 className="text-3xl font-bold text-white">
                LOCADORA MILK
              </h1>
              <p className="text-blue-200 text-sm font-bold">
                Sua Locadora Virtual
              </p>
            </div>
          </div>
          
          <nav className="hidden md:flex space-x-4">
            <button className="bg-blue-600 hover:bg-blue-500 text-white font-bold py-2 px-4">
              FILMES
            </button>
            <button className="bg-gray-600 hover:bg-gray-500 text-white font-bold py-2 px-4">
              MEUS ALUGUÉIS
            </button>
            <button className="bg-blue-600 hover:bg-blue-500 text-white font-bold py-2 px-4">
              CONTATO
            </button>
          </nav>
        </div>
      </div>
    </header>
  );
};

export default Header;
