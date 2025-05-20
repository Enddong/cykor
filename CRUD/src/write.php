<?php
include("includes/db.php");
include("templates/header.php");

if (!isset($_SESSION['userid'])) {
    header("Location: login.php");
    exit;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $title = $_POST['title'];
    $content = $_POST['content'];
    $userId = $_SESSION['userid'];

    mysqli_query($mydb, "INSERT INTO posts (user_id, title, content) VALUES ($userId, '$title', '$content')");
    header("Location: board.php");
    exit;
}
?>

<h2>글 작성</h2>
<form method="POST" action="write.php">
    제목: <input type="text" name="title"><br><br>
    내용:<br>
    <textarea name="content" rows="10" cols="50"></textarea><br><br>
    <input type="submit" value="작성하기">
</form>

<?php include("templates/footer.php"); ?>
