import Link from 'next/link'
import styles from './post-overview.module.css'

type Props = {
  title: string,
  overview: string,
  slug: string,
}

export const PostOverview = ({title, overview, slug}: Props) => (
  <div className={styles.container}>
    <p className={styles.date}>August 7, 2021</p>
    <span className={styles.majorLink}>
      <Link href={"/" + slug}>{title}</Link>
    </span>
    <div className={styles.linkContainer}>
      <Link href={"/" + slug}>NEXT-JS</Link>
      <Link href={"/" + slug}>TAILWIND</Link>
      <Link href={"/" + slug}>GUIDE</Link>
    </div>
    <p>{overview}</p>
    <Link href={"/" + slug}>Read more</Link>
  </div>
)