import Link from 'next/link'
import styles from './PostOverview.module.css'

type Props = {
  title: string,
  overview: string,
  slug: string,
  date: string,
  tag0: string,
  tag1: string,
  tag2: string,
}

export const PostOverview = ({title, overview, slug, date, tag0, tag1, tag2}: Props) => (
  <div className={styles.container}>
    <p className={styles.date}>{date}</p>
    <span className={styles.majorLink}>
      <Link href={"/" + slug}>{title}</Link>
    </span>
    <div className={styles.linkContainer}>
      <Link href={"/" + slug}>{tag0}</Link>
      <Link href={"/" + slug}>{tag1}</Link>
      <Link href={"/" + slug}>{tag2}</Link>
    </div>
    <p>{overview}</p>
    <Link href={"/" + slug}>Read more</Link>
  </div>
)