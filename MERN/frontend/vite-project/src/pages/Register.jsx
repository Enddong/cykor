import { useState } from 'react';
import axios from '../api/axios';

function Register() {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');

  const handleRegister = async (e) => {
    e.preventDefault();
    try {
      await axios.post('/users/register', { username, password });
      alert('회원가입 성공');
    } catch (err) {
      alert('회원가입 실패');
    }
  };

  return (
    <form onSubmit={handleRegister}>
      <h2>회원가입</h2>
      <input placeholder="아이디" value={username} onChange={(e) => setUsername(e.target.value)} />
      <input type="password" placeholder="비밀번호" value={password} onChange={(e) => setPassword(e.target.value)} />
      <button type="submit">가입</button>
    </form>
  );
}

export default Register;
