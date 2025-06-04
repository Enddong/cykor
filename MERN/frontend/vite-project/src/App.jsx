import { useState } from 'react';
import { Routes, Route } from 'react-router-dom';

import Header from './components/Header';
import Footer from './components/Footer';

import Home from './pages/Home';
import Login from './pages/Login';
import Register from './pages/Register';
import WritePost from './pages/WritePost';
import PostList from './pages/PostList';
import EditPost from './pages/EditPost';
import BoardPage from './pages/BoardPage';

import './App.css';

function App() {
  const [user, setUser] = useState(null);
  const [view, setView] = useState(null);

  const handleLogin = (userData) => {
    setUser(userData);
    setView(null);
  };

  const handleLogout = () => {
    setUser(null);
    setView(null);
  };

  return (
    <>
      <Header
        user={user}
        onLogout={handleLogout}
        onLoginClick={() => setView('login')}
        onRegisterClick={() => setView('register')}
      />

      <Routes>
        <Route path="/" element={<Home />} />
        <Route
          path="/board"
          element={
            <BoardPage
              user={user}
              onLogin={handleLogin}
              view={view}
            />
          }
        />
        <Route path="/edit/:id" element={<EditPost />} />
      </Routes>

      <Footer />
    </>
  );
}

export default App;
