<?php
ob_start();
session_start();
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>게시판</title>
</head>
<body>
<a href="index.php">홈</a> |
<a href="board.php">게시판</a>
<?php if (isset($_SESSION['userid'])): ?>
    | <a href="write.php">글쓰기</a>
    | <a href="logout.php">로그아웃</a>
<?php else: ?>
    | <a href="login.php">로그인</a>
    | <a href="register.php">회원가입</a>
<?php endif; ?>
<hr>
