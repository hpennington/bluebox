import Head from 'next/head'
import Image from 'next/image'
import Link from 'next/link'
import styles from './header.module.css'

export const Header = () => {
  return (
    <div className={styles.logo}>
      <h3><Link href="/blog">Bluebox blog</Link></h3>
    </div>
  )
}