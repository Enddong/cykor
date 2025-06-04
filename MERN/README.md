# MERN 스택으로 CRUD 게시판 만들기

2023350031 김은채
MongoDB, Express, React, Node.js를 활용해서 CRUD 게시판을 만들었습니다.
Docker와 Docker Compose를 이용했습니다. 

## 주요 기능

필수적인 기능만을 구현했습니다.
헤더에 홈, 게시판, 로그인, 회원가입 버튼
홈은 초기 방문시 나타나는 페이지
게시판은 게시글 목록을 볼 수 있는 페이지
회원가입 후 로그인을 하면 글 게시, 수정, 삭제가 가능함
로그아웃 상태에서는 게시글 열람만 가능

## 개발 과정에서의 개인적인 목표 

웹 1주차에 만들었던 LAMP 스택의 CRUD 게시판을 MERN으로 바꾸는 것에 집중하였습니다.
기본 기능이나 CSS 등을 똑같이 유지하려고 노력하였습니다.

## backend 설명

index.js -> Express 진입점, MongoDB 연결, API 라우팅 등록 등의 역할
models/Post.js -> 게시글 스키마 정의
models/Users.js -> 유저 스키마 정의의
controllers/postControllers.js -> 게시글 CRUD 처리하는 함수 정의
controllers/userControllers.js -> 유저 회원가입, 로그인 처리
routes/postRoutes.js -> /api/posts 라우팅 처리
routes/userRoutes.js -> /api/users 라우팅 처리

## frontend 설명

App.jsx -> 라우터 설정 (React 라우터로 각 페이지를 연결)
main.jsx -> React 진입점
index.html -> 실제 HTML 템플릿
api/axios.js -> Axios 인스턴스 설정 (API 기본 URL, 토큰 헤더 등)
component/Header.jsx -> 상단 고정 요소 (홈, 게시판, 로그인, 회원가입 등)
compoenet/Footer.jsx -> 하단 고정 요소 (학번 등)
pages/Home.jsx -> 기본 홈화면
pages/Login.jsx -> 로그인 처리 페이지
pages/Register.jsx -> 회원가입 처리 페이지
pages/BoardPage.jsx -> 게시판 기본 레이아웃 페이지
pages/PostList.jsx -> 게시글 목록 조회, 관리 버튼 표시 역할
pages/WritePost.jsx -> 글 작성 페이지
pages/EditPost.jsx -> 글 수정 페이지


## 과제하며 느낀 점

LAMP 스택이 직관적으로 이해하기는 더 쉬웠습니다. 
따라서 처음 MERN으로 구현 당시 어렵다고 느꼈습니다. 
특히 초반 개발 환경 구축, 시스템 환경 변수 설정 등에서 시간을 많이 썼습니다.
그럼에도 MERN 스택에서는 backend랑 frontend로 나누어서 구현한 점이 효율적이라고 느껴졌습니다. 
에러 메세지를 보고 back 측 에러인지, front 측 에러인지, mongoDB에서 온 에러인지 확인할 수 있었기에 해당 하는 부분에서 차근차근 수정을 하기 수월했습니다. 

## 참고자료

https://fabric0de.tistory.com/53

https://github.com/Hayyanshaikh/crud

https://github.com/adhir2099/MERN-CRUD

https://ramincoding.tistory.com/entry/React-Axios-%EA%B0%84%EB%8B%A8%ED%95%98%EA%B2%8C-%EC%82%AC%EC%9A%A9%ED%95%98%EA%B8%B0

https://blog.naver.com/codeitofficial/223497686144

https://velog.io/@bbbgoat/8%EC%9D%BC%EC%B0%A8-Node.js-MongoDB-Part-3-.env-%ED%8C%8C%EC%9D%BC%EC%97%90%EC%84%9C-%EB%AF%BC%EA%B0%90%ED%95%9C-%ED%99%98%EA%B2%BD%EB%B3%80%EC%88%98%EB%93%A4-%EA%B4%80%EB%A6%AC%ED%95%98%EA%B8%B0

