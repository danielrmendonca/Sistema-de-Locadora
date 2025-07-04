const Header = () => {
  return (
    <header className="bg-[#610a59] shadow-lg h-24 flex items-center">
      <div className="container mx-auto px-4">
        <div className="flex items-center justify-between">
          <div className="flex items-center space-x-3">
            <img
              src="/logo.png"
              alt="Logo Locadora Acinsta"
              className="h-20 w-auto"
            />
            <div>
              
              <h1 className="text-3xl font-bold text-white">
                <span style={{ color: '#FFFFFF' }}>LOCADORA</span>{' '}
                <span style={{ color: '#FFFFFF' }}>A</span>
                <span style={{ color: '#FAB7EB' }}>CIN</span>
                <span style={{ color: '#FFFFFF' }}>STA</span>
              </h1>
              <p className="text-blue-200 text-sm font-bold">
                <span style={{ color: '#BC10AD' }}>A locadora oficial do CIN!</span>{' '}
              </p>
            </div>
          </div>

          <nav className="hidden md:flex space-x-4">
            {/* A alteração de cor foi aplicada apenas aqui */}
            <button className="bg-[#A8109B] hover:brightness-90 text-white font-bold py-2 px-4 rounded-md">
              FILMES
            </button>
            <button className="bg-[#A8109B] hover:brightness-90 text-white font-bold py-2 px-4 rounded-md">
              MEUS ALUGUÉIS
            </button>
            <button className="bg-[#A8109B] hover:brightness-90 text-white font-bold py-2 px-4 rounded-md">
              CONTATO
            </button>
          </nav>
        </div>
      </div>
    </header>
  );
};

export default Header;