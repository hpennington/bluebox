import styles from './post-overview.module.css'

export const PostOverview = () => (
  <div className={styles.container}>
    <p className={styles.date}>August 7, 2021</p>
    <a href="" className={styles.majorLink}>New features in v1</a>
    <div className={styles.linkContainer}>
      <a href="">NEXT-JS</a>
      <a href="">TAILWIND</a>
      <a href="">GUIDE</a>
    </div>
    <p>An overview of the new features released in v1 - code block copy, multiple authors, frontmatter layout and more</p>
    <a href="">Read more</a>
  </div>
)