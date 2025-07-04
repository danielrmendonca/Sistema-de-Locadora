import Index from './pages/Index';
import './index.css';

function App() {
  return (

    <div className="flex flex-col min-h-screen bg-gray-50">

      <main className="flex-grow">
        <Index />
      </main>
    </div>
  );
}

export default App;