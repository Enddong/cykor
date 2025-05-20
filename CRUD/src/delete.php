<?php
include("includes/db.php");
include("templates/header.php");

if (!isset($_SESSION['userid'])) {
    header("Location: login.php");
    exit;
}

$id = $_GET['id'];
mysqli_query($mydb, "DELETE FROM posts WHERE id=$id AND user_id=" . $_SESSION['userid']);
?>

<h2>삭제되었습니다.</h2>
<p><a href="board.php">목록으로 돌아가기</a></p>

<?php include("templates/footer.php"); ?>
