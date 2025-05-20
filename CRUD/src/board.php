<?php
include("includes/db.php");
include("templates/header.php");

$query = "
    SELECT posts.id, posts.title, posts.writetime, users.username, posts.user_id
    FROM posts
    JOIN users ON posts.user_id = users.id
    ORDER BY posts.writetime DESC
";
$result = mysqli_query($mydb, $query);
?>

<h2>게시글 목록</h2>

<table border="1" cellpadding="10">
    <tr>
        <th>번호</th>
        <th>제목</th>
        <th>작성자</th>
        <th>작성일</th>
        <?php if (isset($_SESSION['userid'])): ?>
            <th>관리</th>
        <?php endif; ?>
    </tr>

    <?php while ($row = mysqli_fetch_assoc($result)) : ?>
        <tr>
            <td><?php echo $row['id']; ?></td>
            <td>
                <a href="post.php?id=<?php echo $row['id']; ?>">
                    <?php echo htmlspecialchars($row['title']); ?>
                </a>
            </td>
            <td><?php echo htmlspecialchars($row['username']); ?></td>
            <td><?php echo $row['writetime']; ?></td>
            <?php if (isset($_SESSION['userid'])): ?>
                <td>
                    <?php if ($_SESSION['userid'] == $row['user_id']): ?>
                        <a href="edit.php?id=<?php echo $row['id']; ?>">수정</a> |
                        <a href="delete.php?id=<?php echo $row['id']; ?>"
                           onclick="return confirm('정말 삭제하시겠습니까?');">삭제</a>
                    <?php endif; ?>
                </td>
            <?php endif; ?>
        </tr>
    <?php endwhile; ?>
</table>

<?php include("templates/footer.php"); ?>
